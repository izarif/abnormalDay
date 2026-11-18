workspace("sauerbraten")
  configurations{"debug", "release"}
  platforms{"32bit", "64bit"}

  filter("platforms:32bit")
  architecture("x86")

  filter("platforms:64bit")
  architecture("x64")

  filter("configurations:debug")
  symbols("On")
  defines{
    "ROCKET_DEBUG",
  }

  filter("configurations:release")
  optimize("Speed")
  defines{
    "NDEBUG",
  }

project("game")
  language("C++")
  kind("WindowedApp")
  exceptionhandling("Off")
  floatingpoint("Fast")
  includedirs{
    "shared/",
    "engine/",
    "fpsgame/",
  }

  files{
    "shared/crypto.cpp",
    "shared/geom.cpp",
    "shared/glemu.cpp",
    "shared/stream.cpp",
    "shared/tools.cpp",
    "shared/zip.cpp",
    "engine/3dgui.cpp",
    "engine/bih.cpp",
    "engine/blend.cpp",
    "engine/blob.cpp",
    "engine/client.cpp",
    "engine/command.cpp",
    "engine/console.cpp",
    "engine/cubeloader.cpp",
    "engine/decal.cpp",
    "engine/dynlight.cpp",
    "engine/glare.cpp",
    "engine/grass.cpp",
    "engine/lightmap.cpp",
    "engine/main.cpp",
    "engine/material.cpp",
    "engine/menus.cpp",
    "engine/movie.cpp",
    "engine/normal.cpp",
    "engine/octa.cpp",
    "engine/octaedit.cpp",
    "engine/octarender.cpp",
    "engine/physics.cpp",
    "engine/pvs.cpp",
    "engine/rendergl.cpp",
    "engine/rendermodel.cpp",
    "engine/renderparticles.cpp",
    "engine/rendersky.cpp",
    "engine/rendertext.cpp",
    "engine/renderva.cpp",
    "engine/server.cpp",
    "engine/serverbrowser.cpp",
    "engine/shader.cpp",
    "engine/shadowmap.cpp",
    "engine/sound.cpp",
    "engine/texture.cpp",
    "engine/water.cpp",
    "engine/world.cpp",
    "engine/worldio.cpp",
    "fpsgame/ai.cpp",
    "fpsgame/client.cpp",
    "fpsgame/entities.cpp",
    "fpsgame/fps.cpp",
    "fpsgame/monster.cpp",
    "fpsgame/movable.cpp",
    "fpsgame/render.cpp",
    "fpsgame/scoreboard.cpp",
    "fpsgame/server.cpp",
    "fpsgame/waypoint.cpp",
    "fpsgame/weapon.cpp",
    "engine/rGui/ShellFileInterface.cpp",
    "engine/rGui/SystemInterfaceSDL2GL.cpp",
    "engine/rGui/RenderInterfaceSDL2GL.cpp",
    "engine/rGui/rGui.cpp",
  }

  filter{"system:windows", "platforms:32bit"}
  libdirs{
    "lib",
  }

  filter{"system:windows", "platforms:64bit"}
  libdirs{
    "lib64",
  }

  filter{"system:windows", "action:gmake", "platforms:32bit"}
  resoptions{
    "-F pe-i386",
  }

  filter("system:windows")
  files{
    "vcpp/game.rc",
  }

  includedirs{
    "include/",
    "include/zlib/",
    "include/SDL2/",
  }

  links{
    "opengl32",
    "ws2_32",
    "winmm",
    "z",
    "enet",
    "SDL2main",
    "SDL2",
    "SDL2_image",
    "SDL2_mixer",
    "RocketCore",
    "RocketControls",
    "RocketDebugger",
  }

  filter("system:macosx")
  links{
    "Cocoa.framework",
    "OpenGL.framework",
    "CoreAudio.framework",
    "AudioToolbox.framework",
    "AudioToolbox.framework",
    "AudioUnit.framework",
    "z",
    "enet",
    "SDL2.framework",
    "SDL2_image.framework",
    "SDL2_mixer.framework",
    "RocketCore",
    "RocketControls",
    "RocketDebugger",
  }

  filter("system:not windows", "system:not macosx")
  links{
    "GL",
    "X11",
    "rt",
    "z",
    "enet",
    "SDL2",
    "SDL2_image",
    "SDL2_mixer",
    "RocketCore",
    "RocketControls",
    "RocketDebugger",
  }
