# Add core source files to the engine target
target_sources(engine PRIVATE
    KitsuneEngine.cpp
    KitsuneEngine.h
)

# Make this folder available for includes
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

