# Engine architecture

AngelBase is a 3D multithreaded ECS game engine, a game engine that seeks to take advantage of new operating system features

## Renderer Architecture
For the sake of simplicity, we should use a uniform interface API for the renderer. This will be a C++ interface class, to simplify the mental overhead of using the rendering system. Beneath that, we will use a C-style interface, where the dependencies that each C-style function depends on are explicitly laid out. This is to ensure that we are explicit about what each function needs to depend on, which is useful for submitting jobs for the dependency graphs.
