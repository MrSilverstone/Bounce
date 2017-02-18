cmake_minimum_required(VERSION 2.8)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

set(SHORT_NAME Bounce)
project(${SHORT_NAME})
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "Bounce")
set(VITA_TITLEID  "VSDK00017")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
  include
  libs/include
  )

link_directories(
  ${PROJECT_SOURCE_DIR}/libs
  )

set(VERTEX_SHADERS
  shader/clear_v.cg
  shader/color_v.cg
  shader/light_v.cg
  )

set(FRAGMENT_SHADERS
  shader/clear_f.cg
  shader/color_f.cg
  shader/light_f.cg
  )

set(DATA

  )

foreach(shader ${VERTEX_SHADERS})
  get_filename_component(shader_we ${shader} NAME_WE)
  add_custom_command(OUTPUT "${shader_we}.gxp"
    COMMAND ../shader/psp2cg.sh --vertex "${PROJECT_SOURCE_DIR}/${shader}"
    "${CMAKE_BINARY_DIR}/${shader_we}.gxp"
    DEPENDS ${shader}
    COMMENT "Compiling ${shader} to ${shader_we}.gxp"
    WORKING_DIRECTORY ../shader
    )
  list(APPEND SHADER_GXPS "${shader_we}.gxp")
endforeach()

foreach(shader ${FRAGMENT_SHADERS})
  get_filename_component(shader_we ${shader} NAME_WE)
  add_custom_command(OUTPUT "${shader_we}.gxp"
    COMMAND ../shader/psp2cg.sh --fragment "${PROJECT_SOURCE_DIR}/${shader}"
    "${CMAKE_BINARY_DIR}/${shader_we}.gxp"
    DEPENDS ${shader}
    COMMENT "Compiling ${shader} to ${shader_we}.gxp"
    WORKING_DIRECTORY ../shader
    )
  list(APPEND SHADER_GXPS "${shader_we}.gxp")
endforeach()

foreach(gxp ${SHADER_GXPS})
  get_filename_component(gxp_we ${gxp} NAME_WE)
  add_custom_command(OUTPUT "${gxp_we}_gxp.o"
    COMMAND arm-vita-eabi-objcopy --input binary --output elf32-littlearm
    --binary-architecture arm "${gxp}"
    "${CMAKE_BINARY_DIR}/${gxp_we}_gxp.o"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    DEPENDS ${gxp}
    COMMENT "Objcopying ${gxp} to ${gxp_we}_gxp.o"
    )
  list(APPEND SHADER_OBJECTS "${gxp_we}_gxp.o")
  endforeach()

add_executable(${SHORT_NAME}
  src/main.cpp
  src/Mesh.cpp
  src/Utils.cpp
  src/Camera.cpp
  src/Texture.cpp
  src/Shader.cpp
  src/ShaderFactory.cpp
  src/Context.cpp
  src/Scene.cpp
  src/ShaderFactory.cpp
  src/ScreenClearer.cpp
  src/AssetsManager.cpp
  src/ScriptSystem.cpp
  ${SHADER_OBJECTS}
  )

target_link_libraries(${SHORT_NAME}
  png
  jpeg
  m
  z
  c
  freetype
  SceDisplay_stub
  SceGxm_stub
  SceCtrl_stub
  SceAudio_stub
  debugnet
  SceSysmodule_stub
  SceNet_stub
  SceNetCtl_stub
  bullet
  )

vita_create_self(${SHORT_NAME}.self ${SHORT_NAME})
vita_create_vpk(${SHORT_NAME}.vpk ${VITA_TITLEID} ${SHORT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE ${CMAKE_BINARY_DIR}/clear_f.gxp res/Shaders/clear_f.gxp
  FILE ${CMAKE_BINARY_DIR}/clear_v.gxp res/Shaders/clear_v.gxp
  FILE ${CMAKE_BINARY_DIR}/color_f.gxp res/Shaders/color_f.gxp
  FILE ${CMAKE_BINARY_DIR}/color_v.gxp res/Shaders/color_v.gxp
  FILE ${CMAKE_BINARY_DIR}/light_f.gxp res/Shaders/light_f.gxp
  FILE ${CMAKE_BINARY_DIR}/light_v.gxp res/Shaders/light_v.gxp
  FILE ${PROJECT_SOURCE_DIR}/res/sphere.obj res/Mesh/sphere.obj
  )

add_custom_target(vpksend
  COMMAND curl -T ${SHORT_NAME}.vpk ftp://$(PSVITAIP):1337/ux0:/
  DEPENDS ${SHORT_NAME}.vpk
  )

add_custom_target(send
  COMMAND curl -T ${SHORT_NAME}.self ftp://$(PSVITAIP):1337/ux0:/app/${VITA_TITLEID}/eboot.bin
  DEPENDS ${SHORT_NAME}.self
)
