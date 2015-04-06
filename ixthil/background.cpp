#include "background.h"

Background::Background(const Rect &dims)
	:m_dims(dims),
	 m_fore(),
	 m_back(),
	 m_star_speed(450),
	 m_hue(0),
	 m_cells(NULL),
	 m_new_cells(NULL),
	 m_cell_w(5),
	 m_cell_h(5),
	 m_w(dims[2] / m_cell_w),
	 m_h(dims[3] / m_cell_h),
	 m_update_timer(0),
	 m_random_timer(0),
	 m_max_mag(0.03),
	 m_speed(1.0),
	 m_fuzzy(1),
	 m_max_brightness(64.0),
	 m_subspace(false),
	 m_hues(NULL),
	 m_cells_updated(0),
	 m_bg_surface(NULL)
{
	for (int i = 0; i < 300; ++i)
	{
		vector2d star(randint(0, dims[2] - 1),
		              randint(0, dims[3] - 1));
		if (i % 4 == 0)
			m_fore.push_back(star);
		else
			m_back.push_back(star);
	}

	fprintf(stderr, "Making a background with %d cells\n", m_w * m_h);
	
	m_cells = (bg_cell_t *)malloc(sizeof(bg_cell_t) * m_w * m_h);
	m_new_cells = (bg_cell_t *)malloc(sizeof(bg_cell_t) * m_w * m_h);
	for (int x = 0; x < m_w; ++x)
		for (int y = 0; y < m_h; ++y)
			init_cell(get_cell(x, y));

	m_hues = (Uint32 *)malloc(sizeof(Uint32) * 360 * 255);
	for (int hue = 0; hue < 360; ++hue)
		for (int b = 0; b < 255; ++b)
			m_hues[hue * 255 + b] = map_hueB(hue, b);

	m_bg_surface = new Surface(m_w * m_cell_w, m_h * m_cell_h);
	m_bg_surface->fill(0);
}

void Background::init_cell(bg_cell_t *cell)
{
	cell->value = -1;
	cell->life = 0;
	cell->color = mapRGB(0, 0, 0);
	cell->hue = 0;
	cell->hue_updated = false;
}

Background::~Background()
{
	delete m_bg_surface;
	free(m_hues);
	free(m_cells);
	free(m_new_cells);
}

void Background::pause()
{
	m_update_timer.pause();
}

void Background::unpause()
{
	m_update_timer.unpause();
}

void Background::update(double dt)
{
	m_cells_updated = 0;

	/* update the stars */
	list<vector2d>::iterator i;
	for (i = m_fore.begin(); i != m_fore.end(); i++)
	{
		(*i)[1] += m_star_speed * dt;
		if ((*i)[1] >= m_dims[3])
		{
			(*i)[1] -= m_dims[3];
			(*i)[0] = randint(0, m_dims[2] - 1) - randint(0, 25);
		}
	}
	for (i = m_back.begin(); i != m_back.end(); i++)
	{
		(*i)[1] += 0.5 * m_star_speed * dt;
		if ((*i)[1] >= m_dims[3])
		{
			(*i)[1] -= m_dims[3];
			(*i)[0] = randint(0, m_dims[2] - 1) - randint(0, 25);
		}
	}

	/* wait until we need to update the entire thing */
	if (!m_update_timer.done())
		return;

	/* if in subspace then randomly set a
	   pixel to the space color
	*/
	if (m_subspace && m_random_timer.done())
	{
		bg_cell_t *cell;
		int x = randint(1, m_w);
		int y = randint(1, m_h);
		cell = get_cell(x, y);
		cell->value = m_hue;
		alert_cell(x, y);
		m_random_timer.reset(geometric(1.0 / 100.0) / 30);
	}

	/* update cells */
	for (int x = 0; x < m_w; ++x)
		for (int y = 0; y < m_h; ++y)
			update_cell(x, y);

	/* swap arrays */
	bg_cell_t *tmp = m_cells;
	m_cells = m_new_cells;
	m_new_cells = tmp;

	/* update render color of each cell */
	Uint32 black = mapRGB(0, 0, 0);
	for (int x = 0; x < m_w; ++x)
	{
		for (int y = 0; y < m_h; ++y)
		{
			bg_cell_t *cell = get_cell(x, y);
			if (cell->value < 0)
			{
				if (cell->color != black)
				{
					cell->color = black;
					draw_cell(m_bg_surface, x, y);
				}
			}
			else
			{
				int hue = get_hue_cell(x, y);
				int brightness = (m_max_brightness * cell->life);
				brightness /= MAX_CELL_LIFE;
				Uint32 color;
				color = m_hues[hue * 255 + brightness];
				Uint32 old_color = cell->color;
				cell->color = color;
				if (color != old_color)
					draw_cell(m_bg_surface, x, y);
			}
		}
	}

	//fprintf(stdout, "%d\n", m_cells_updated);
	
	m_update_timer.reset(1.0 / 30.0);
}

void Background::update_cell(int x, int y)
{
	bg_cell_t *cell = get_cell(x, y);
	bg_cell_t *dst = get_new_cell(x, y);
	
	bool changed = false;

	int old = cell->value;
	dst->value = get_dominant(x, y);
	dst->hue = cell->hue;

	if (dst->value != old)
		changed = true;
	
	if (m_subspace)
		dst->life = MAX_CELL_LIFE;
	else
	{
		dst->life = max(0, avg_life(x, y) - 1);

		if (dst->life <= 0)
		{
			dst->value = -1;
			dst->life = 0;
			changed = true;
		}
	}

	if (changed)
		alert_cell(x, y);
}

void Background::alert_cell(int x, int y)
{
	for (int dx = -m_fuzzy; dx <= m_fuzzy; ++dx)
		for (int dy = -m_fuzzy; dy <= m_fuzzy; ++dy)
			get_new_cell(x + dx, y + dy)->hue_updated = true;
}

void Background::draw(Surface *dst) const
{
	dst->blit(m_bg_surface, 0, 0);

	Uint32 c1 = mapRGB(127, 127, 127);
	Uint32 c2 = mapRGB(63, 63, 63);
	list<vector2d>::const_iterator i;
	for (i = m_fore.begin(); i != m_fore.end(); i++)
		dst->draw_point(c1, (*i)[0], (*i)[1]);
	for (i = m_back.begin(); i != m_back.end(); i++)
		dst->draw_point(c2, (*i)[0], (*i)[1]);
}

void Background::enter_subspace(const vector2d &point, int value)
{
	int x = point[0] / m_cell_w;
	int y = point[1] / m_cell_h;
	int k = 6;
	for (int dx = -k; dx <= k; ++dx)
		for (int dy = -k; dy <= k; ++dy)
			if (fabs(dx) + fabs(dy) <= k)
				get_cell(x + dx, y + dy)->value = value;

	m_hue = value;
	m_subspace = true;
}

void Background::exit_subspace()
{
	m_subspace = false;
}

bg_cell_t *Background::get_array(bg_cell_t *array,
                                 int x, int y) const
{
	x = (x + m_w) % m_w;
	y = (y + m_h) % m_h;
	return array + m_w * y + x;
}

bg_cell_t *Background::get_cell(int x, int y) const
{
	return get_array(m_cells, x, y);
}

bg_cell_t *Background::get_new_cell(int x, int y) const
{
	return get_array(m_new_cells, x, y);
}

int Background::get_hue_cell(int x, int y)
{
	bg_cell_t *cell = get_cell(x, y);
	if (!cell->hue_updated)
		return cell->hue;

	m_cells_updated += 1;
	
	int n = 2 * m_fuzzy + 1;
	int area = n * n;
	int theta[area];
	int vals = 0;
	
	for (int dx = -m_fuzzy; dx <= m_fuzzy; ++dx)
	{
		for (int dy = -m_fuzzy; dy <= m_fuzzy; ++dy)
		{
			bg_cell_t *cur_cell = get_cell(x + dx, y + dy);
			if (cur_cell->value >= 0)
				theta[vals++] = 60 * cur_cell->value;
		}
	}
	
	int ret = avg_theta(theta, vals);
	cell->hue = ret;
	cell->hue_updated = false;
	return ret;
}

void Background::draw_cell(Surface *dst, int x, int y) const
{
	bg_cell_t *cell = get_cell(x, y);
	Uint32 color = cell->color;
	Rect rect(m_cell_w * x, m_cell_h * y, m_cell_w, m_cell_h);
	dst->draw_rect(color, rect);
}

int Background::get_dominant(int x, int y) const
{
	int values[10];
	int num_vals = 0;
	int num_hue = 0;
	
	int val;
	for (int dx = -1; dx <= 1; ++dx)
	{
		for (int dy = -1; dy <= 1; ++dy)
		{
			val = get_cell(x + dx, y + dy)->value;
			if (val >= 0)
			{
				values[num_vals++] = val;
				if (val == m_hue)
					num_hue++;
			}
		}
	}

	if (num_vals <= 0)
		return -1;

	if (num_hue >= 5)
		values[num_vals++] = m_hue;

	int k = randint(1, num_vals) - 1;
	return values[k];
}

int Background::avg_life(int x, int y) const
{
	int k = 1;
	int n = 2 * k + 1;
	int area = n * n;
	int vals[area];
	int i = 0;
	for (int dx = -k; dx <= k; ++dx)
		for (int dy = -k; dy <= k; ++dy)
			vals[i++] = get_cell(x + dx, y + dy)->life;
	return avg_value(vals, area);
}

void Background::add_value(int value, double x, double y)
{
	if (value < 0)
		return;
	
	int k = 1;
	int real_x = x / m_cell_w;
	int real_y = y / m_cell_h;
	if (real_x < k || real_y < k ||
	    real_x >= m_w - k || real_y >= m_h - k)
		return;

	for (int dx = -k; dx <= k; ++dx)
	{
		for (int dy = -k; dy <= k; ++dy)
		{
			int cur_x = real_x + dx;
			int cur_y = real_y + dy;
			bg_cell_t *cell = get_cell(cur_x, cur_y);
			if (value != cell->value)
				alert_cell(cur_x, cur_y);
			cell->value = value;
			cell->life = MAX_CELL_LIFE;
		}
	}
}
