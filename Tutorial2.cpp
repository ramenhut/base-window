/*
//
// Copyright (c) 1998-2019 Joe Bertolami. All Right Reserved.
//
//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//   AND ANY EXPRESS OR IMPLIED WARRANTIES, CLUDG, BUT NOT LIMITED TO, THE
//   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//   ARE DISCLAIMED.  NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//   LIABLE FOR ANY DIRECT, DIRECT, CIDENTAL, SPECIAL, EXEMPLARY, OR
//   CONSEQUENTIAL DAMAGES (CLUDG, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
//   GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSESS TERRUPTION)
//   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER  CONTRACT, STRICT
//   LIABILITY, OR TORT (CLUDG NEGLIGENCE OR OTHERWISE) ARISG  ANY WAY  OF THE
//   USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Additional Information:
//
//   For more information, visit http://www.bertolami.com.
//
*/

/* Tutorial2.cpp: how to create a simple window and handle input. */

#include <iostream>
#include <memory>
#include <vector>
#include "base_window.h"

using namespace base;
using ::std::make_unique;
using ::std::unique_ptr;
using ::std::vector;

int main(int argc, char** argv) {
  /* Create a queue for input events. */
  vector<InputEvent> window_events;

  /* create our window */
  auto window = make_unique<BaseWindow>("Tutorial 2", 100, 10, 800, 600);

  /* while the window is valid, update it and perform our app logic. */
  while (window && window->IsValid()) {
    window->Update(&window_events);

    /* Handle all of our recent events that collected in our queue. */
    for (auto& event : window_events) {
      /* If the user pressed escape, exit the app. */
      if (event.switch_index == 27) {
        return 0;
      }
      /* Check if the left mouse was clicked and report its values if so.
         Mouse coordinates range from [-1, 1] with (0, 0) being the center
         of the screen. */
      if (event.switch_index == ::base::kInputMouseLeftButtonIndex) {
        ::std::cout << "Left mouse " << (event.is_on ? "down" : "up") << " @ "
                    << event.target_x << ", " << event.target_y << ::std::endl;
      }
    }

    /* Your application logic goes here. */
  }

  return 0;
}