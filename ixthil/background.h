#ifndef __BACKGROUND_H
#define __BACKGROUND_H

#include "color.h"
#include "surface.h"
#include "timer.h"

#define MAX_CELL_LIFE 60

typedef struct bg_cell_type
{
	int value;
	int life;
	Uint32 color;
	int hue;
	bool hue_updated;
} bg_cell_t;

class Background
{
public:
	Background(const Rect &dims);
	~Background();

	void pause();
	void unpause();

	void update(double dt);
	void draw(Surface *dst) const;

	void enter_subspace(const vector2d &point, int value);
	void exit_subspace();
	void add_value(int value, double x, double y);
private:
	bg_cell_t *get_array(bg_cell_t *array,
	                     int x, int y) const;
	bg_cell_t *get_cell(int x, int y) const;
	bg_cell_t *get_new_cell(int x, int y) const;
	void init_cell(bg_cell_t *cell);
	int get_hue_cell(int x, int y);
	void draw_cell(Surface *dst, int x, int y) const;
	int get_dominant(int x, int y) const;
	int avg_life(int x, int y) const;
	void update_cell(int x, int y);
	void alert_cell(int x, int y);

	Rect m_dims;
	list<vector2d> m_fore;
	list<vector2d> m_back;
	double m_star_speed;
	int m_hue;
	bg_cell_t *m_cells;
	bg_cell_t *m_new_cells;
	int m_cell_w;
	int m_cell_h;
	int m_w;
	int m_h;
	Timer m_update_timer;
	Timer m_random_timer;
	double m_max_mag;
	double m_speed;
	int m_fuzzy;
	double m_max_brightness;
	bool m_subspace;

	Uint32 *m_hues;

	int m_cells_updated;
	Surface *m_bg_surface;
};

#endif
