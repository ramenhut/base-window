## BaseWindow & BaseGraphics
Header only C++ code that makes it easy to create a window and draw pretty pictures on it. There's lots of frameworks out there that do this, but few that do **only** this *one*. *simple*. *thing*. Let's not boil the ocean!

This is actually a simplified and minified version of the window management logic in the [Vision game engine](http://bertolami.com/index.php?engine=portfolio&content=game-development&detail=project-vision-game-engine-2-0). Vision supports MacOS, Windows, Linux, iOS, and Android, but this release is currently limited to Windows only. 

Additional platform support will be added as time allows.

## Instructions
To get started include **base_window.h** (if you only want windowing), or **base_graphics.h** (if you also want to draw using OpenGL). Follow the super squeaky examples below:

#### Let's create a window:
```C++
  /* create our window */
  auto window = make_unique<BaseWindow>("Tutorial 1", 100, 10, 800, 600);

  /* while the window is valid, update it and perform our app logic. */
  while (window && window->IsValid()) {
    window->Update();

    /* Your application logic goes here. */
  }
```

#### Let's handle some input:
```C++
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
      if (event.switch_index == kInputMouseLeftButtonIndex) {
        cout << "Left mouse " << (event.is_on ? "down" : "up") << " @ "
             << event.target_x << ", " << event.target_y << endl;
      }
    }

    /* Your application logic goes here. */
  }
```

#### Let's draw using OpenGL:
```C++
  /* create our window */
  auto window =
      make_unique<GraphicsWindow>("Tutorial 3", 100, 10, 800, 600, 32, 0);

  /* while the window is valid, update it and perform our app logic. */
  while (window && window->IsValid()) {
    window->Update();
    window->BeginScene();

    /* throw some color on our window. */
    glClearColor(0.75f, 0.5f, 1.0f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Your OpenGL commands go here. */

    window->EndScene();
  }
```

## Details

This software is released under the terms of the BSD 2-Clause “Simplified” License.

For more information visit [http://www.bertolami.com](http://bertolami.com).