/*
** main.cc
** Copyright (C) 2008 Jean-Baptiste MOURET
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <iostream>
#include "fastsim.hpp"

int main(int argc, char** argv) {
  using namespace fastsim;
  assert(argc == 2);
  fastsim::Settings settings(argv[1]);
  boost::shared_ptr<fastsim::Map> map = settings.map(); // fastsim::Map in template argument so as to not confuse with Eigen::Map
  boost::shared_ptr<Robot> robot = settings.robot();
  boost::shared_ptr<ASV> asv = settings.asv();

  Display d(map, *robot);

  Posture p;

  //p = robot->get_pos();

  asv->set_pos(Vector3f(robot->get_pos().get_x()*0.01f, robot->get_pos().get_y()*0.01f, robot->get_pos().theta())); // convert distance to meters


  //std::cout << "ASV acceleration " << asv->get_acceleration() << std::endl;

  //std::cout << "ASV velocity " << asv->get_velocity() << std::endl;

  //std::cout << "ASV pos " << asv->get_pos() << std::endl;


  for (int i = 0; i < 10000; ++i)
    {
      d.update();

      asv->set_thrust(10.0f, 10.0f, 0.0f);
      asv->inverseDynamics();
      asv->kinematics();
      Vector3f newpos = asv->get_pos();

      //std::cout << "ASV acceleration " << asv->get_acceleration() << std::endl;

      //std::cout << "ASV velocity " << asv->get_velocity() << std::endl;

      //std::cout << "ASV pos " << asv->get_pos() << std::endl; // in meters


      p.set_x(newpos(0)*100.0f); // convert to cm
      p.set_y(newpos(1)*100.0f); // convert to cm
      p.set_theta(newpos(2));
      //robot->set_pos(p);
      robot->move(p, map);


      //robot->move(1.0, 1.0, map);
      //std::cout << robot->get_pos().get_x()  << ", " << robot->get_pos().get_y() << ", " << robot->get_pos().theta() << std::endl;

      //usleep(1000); // why sleep for 1s?

      // updating asv position following possible collisions
      asv->set_pos(Vector3f(robot->get_pos().get_x()*0.01f, robot->get_pos().get_y()*0.01f, robot->get_pos().theta()));
    }
  return 0;
}
// Example of a main without a config file
/*
int main()
{
  try
    {
      using namespace fastsim;
      boost::shared_ptr<Map> m =
    boost::shared_ptr<Map>(new Map("cuisine.pbm", 600));
      m->add_goal(Goal(100, 100, 10, 0));
      Robot r(20.0f, Posture(200, 200, 0));
      r.add_laser(Laser(M_PI / 4.0, 100.0f));
      r.add_laser(Laser(-M_PI / 4.0, 100.0f));
      r.add_laser(Laser(0.0f, 100.0f));
      r.add_radar(Radar(0, 4));
      Display d(m, r);

      float x = 30;
      for (int i = 0; i < 10000; ++i)
    {
      d.update();
      r.move(1.0, 1.1, m);
    }
    }
  catch (fastsim::Exception e)
    {
      std::cerr<<"error : "<<e.get_msg()<<std::endl;
    }
  return 0;
}
*/
