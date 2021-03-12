# polygonnider-learning-opengl
Learning how to render graphics using C++ and OpenGl

## Build
`g++ -o Application.exe src/Application.cpp src/Game.cpp src/Player.cpp -Iinclude -Llib -lglfw3 -lglew32 -lopengl32 -lgdi32 -DGLEW_STATIC`

>You can also add `-D_DEBUG` to have error messages printed
