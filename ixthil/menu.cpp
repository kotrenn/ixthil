#include "menu.h"

#include "datamanager.h"
#include "resourcemanager.h"
#include "system.h"

MenuController::MenuController()
	:m_menu(NULL),
	 m_old_menu(NULL)
{
}

MenuController::~MenuController()
{
	exit_all();
	if (m_old_menu != NULL)
		delete m_old_menu;
}

void MenuController::update()
{
	if (m_menu != NULL)
	{
		m_menu->update();
		m_menu->set_active(true);
	}

	if (m_old_menu != NULL)
	{
		delete m_old_menu;
		m_old_menu = NULL;
	}
}

void MenuController::draw(Surface *dst) const
{
	if (m_menu != NULL)
		m_menu->draw(dst);
}

Menu *MenuController::set_menu(Menu *menu)
{
	Menu *ret = m_menu;
	if (m_menu != NULL)
		m_menu->set_active(false);
	m_menu = menu;
	return ret;
}

void MenuController::exit_menu()
{
	fprintf(stderr, "exit_menu(%x)\n", m_menu);
	fflush(stderr);
	
	m_old_menu = m_menu;
	m_menu = m_menu->get_parent();
	if (m_menu != NULL)
		m_menu->reset();
}

void MenuController::exit_all()
{
	Menu *top_menu = m_menu;
	
	while (m_menu != NULL)
	{
		exit_menu();
		if (m_old_menu != top_menu)
			delete m_old_menu;
	}
	
	m_old_menu = top_menu;
}



Menu::Menu(MenuController *controller,
           InputDevice *device)
	:InputListener(device),
	 m_controller(controller),
	 m_parent(NULL),
	 m_active(true),
	 m_options(),
	 m_cur(0),
	 m_select_timer(0),
	 m_icon(NULL),
	 m_corner_x(0),
	 m_corner_y(0)
{
	m_parent = m_controller->set_menu(this);
	fprintf(stderr, "m_parent = %x\n", m_parent);
	fflush(stdout);
	
	ResourceManager *rm = ResourceManager::get_instance();
	m_icon = rm->get_image("interface/menu_icon.png");
}

void Menu::set_corner(int x, int y)
{
	m_corner_x = x;
	m_corner_y = y;
}

void Menu::reset()
{
	m_cur = 0;
}

void Menu::update()
{
	if (!m_active)
		return;
	
	if (m_select_timer.done())
	{
		int prev_cur = m_cur;
		
		int size = m_options.size();
		if (m_input->pressed(BUTTON_UP))
			m_cur = (m_cur + size - 1) % size;
		else if (m_input->pressed(BUTTON_DOWN))
			m_cur = (m_cur + size + 1) % size;

		if (prev_cur != m_cur)
		{
			DataManager *dm = DataManager::get_instance();
			double delay = dm->get_dbl("player_select_delay");
			m_select_timer.reset(delay);
		}
	}
}

void Menu::draw(Surface *dst) const
{
	ResourceManager *rm = ResourceManager::get_instance();
	Font *font = rm->get_font();

	int option_space = 5;
	int icon_space = 10;

	for (unsigned int i = 0; i < m_options.size(); ++i)
	{
		int str_x = m_corner_x;
		int str_y = m_corner_y + i * (font->get_h() + option_space);
		font->draw_string(dst, str_x, str_y, m_options[i].c_str());
	}

	int icon_x = m_corner_x - m_icon->get_w() - icon_space;
	int icon_y = m_corner_y + m_cur * (font->get_h() + option_space);
	dst->blit(m_icon, icon_x, icon_y);
}

void Menu::button_pressed(Button button)
{
	if (!m_active)
		return;
	
	if (button == BUTTON_SHOOT ||
	    button == BUTTON_PAUSE)
		selection(m_options[m_cur]);
}



MainMenu::MainMenu(MenuController *controller,
                   InputDevice *device,
                   LevelSelector *level_selector)
	:Menu(controller, device),
	 m_level_selector(level_selector)
{
	m_options.push_back("Start Game");
	m_options.push_back("Options");
	m_options.push_back("How to Play");
	m_options.push_back("Exit");

	Level *level = m_level_selector->get_level();
	int corner_x = 0.5 * level->get_w() - 55;
	int corner_y = 0.4 * level->get_h();
	set_corner(corner_x, corner_y);
}

void MainMenu::selection(const string &option)
{
	fprintf(stderr, "MainMenu::selection(%s)\n", option.c_str());
	fflush(stderr);
	
	if (option == "Start Game")
		new StageMenu(m_controller, m_input, m_level_selector);
	else if (option == "Options")
	{
	}
	else if (option == "How to Play")
	{
		Level *level = m_level_selector->get_level();
		level->clear_bullets();
		//m_level_selector->switch_level("help1");
		m_level_selector->switch_level("level1");
		m_controller->exit_menu();
	}
	else if (option == "Exit")
	{
		m_controller->exit_menu();
		System *system = System::get_instance();
		system->exit();
	}
}



StageMenu::StageMenu(MenuController *controller,
                     InputDevice *device,
                     LevelSelector *level_selector)
	:Menu(controller, device),
	 m_level_selector(level_selector)
{
	m_options.push_back("Stage 1");
	m_options.push_back("Stage 2");
	m_options.push_back("Stage 3");
	m_options.push_back("Stage 4");
	m_options.push_back("Stage 5");
	m_options.push_back("Stage 6");
	m_options.push_back("Extra Stage");
	m_options.push_back("Super Extra Stage");
	m_options.push_back("Back");

	Level *level = m_level_selector->get_level();
	int corner_x = 0.5 * level->get_w() - 55;
	int corner_y = 0.4 * level->get_h();
	set_corner(corner_x, corner_y);
}

void StageMenu::selection(const string &option)
{
	fprintf(stderr, "StageMenu::selection(%s)\n", option.c_str());
	fflush(stderr);
	
	if (option == "Back")
	{
		m_controller->exit_menu();
		return;
	}

	PlayerActor *player = m_level_selector->get_player();
	player->end_demo();
	Level *level = m_level_selector->get_level();
	level->clear_bullets();
	if (option == "Stage 1" || true)
		m_level_selector->switch_level("level1");
	m_controller->exit_all();
}



PauseMenu::PauseMenu(MenuController *controller,
                     InputDevice *device,
                     LevelSelector *level_selector)
	:Menu(controller, device),
	 m_level_selector(level_selector)
{
	m_options.push_back("Return to Game");
	m_options.push_back("Options");
	m_options.push_back("Return to Title");

	Level *level = m_level_selector->get_level();
	int corner_x = 0.5 * level->get_w() - 55;
	int corner_y = 0.4 * level->get_h();
	set_corner(corner_x, corner_y);
}

void PauseMenu::selection(const string &option)
{
	if (option == "Return to Game")
	{
		m_controller->exit_menu();
	}
	else if (option == "Options")
	{
	}
	else if (option == "Return to Title")
	{
		m_level_selector->switch_level("title");
		m_controller->exit_menu();
	}
}
