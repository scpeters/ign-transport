/*
 * Copyright (C) 2014 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <iostream>
#include <string>
#include <ignition/msgs.hh>
#include <ignition/transport.hh>

//////////////////////////////////////////////////
/// \brief Provide a "without_input" service.
void srvEcho(ignition::msgs::StringMsg &_rep, bool &_result)
{
  // Set the response's content.
  _rep.set_data("Answer from 'without_input' server");

  // The response succeed.
  _result = true;
}

//////////////////////////////////////////////////
int main(int argc, char **argv)
{
  // Let's print the list of our network interfaces.
  std::cout << "List of network interfaces in this machine:" << std::endl;
  for (const auto &netIface : ignition::transport::determineInterfaces())
    std::cout << "\t" << netIface << std::endl;

  // Create a transport node.
  ignition::transport::Node node;
  std::string service = "/without_input";

  // Advertise a service call.
  if (!node.Advertise(service, srvEcho))
  {
    std::cerr << "Error advertising service [" << service << "]" << std::endl;
    return -1;
  }

  // Zzzzzz.
  ignition::transport::waitForShutdown();
}