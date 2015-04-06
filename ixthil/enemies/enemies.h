#ifndef __ENEMIES_H
#define __ENEMIES_H

#include "simple.h"
#include "strong.h"
#include "attractor.h"
#include "smart.h"
#include "bounce.h"
#include "group.h"
#include "cloud.h"

#include "darksimple.h"
#include "backandforth.h"
#include "darkattractor.h"

#include "quick.h"

class Enemies
{
public:
	static void random_enemies(Level *level);
	
	static void set_cloud_center(const vector2d &center);
	static vector2d get_cloud_center() { return m_cloud_center; }
private:
	static bool m_initialized;
	static const int m_history_size = 2;
	static int m_history[m_history_size];
	static vector2d m_cloud_center;
};

#endif
