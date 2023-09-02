# project (${PADTHV1_SRC_DIR}/PADTHV1)

set (PADTHV1_SRC_DIR "${PROJECT_SOURCE_DIR}/src/src")

include_directories (
    ${PADTHV1_SRC_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

if (EXISTS ${PADTHV1_SRC_DIR}/config.h)
  file (REMOVE ${PADTHV1_SRC_DIR}/config.h)
endif ()
configure_file (${PADTHV1_SRC_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)
configure_file (config_dpf.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config_dpf.h)

set (HEADERS
  ${PADTHV1_SRC_DIR}/padthv1.h
  ${PADTHV1_SRC_DIR}/padthv1_sample.h
  ${PADTHV1_SRC_DIR}/padthv1_config.h
  ${PADTHV1_SRC_DIR}/padthv1_filter.h
  ${PADTHV1_SRC_DIR}/padthv1_formant.h
  ${PADTHV1_SRC_DIR}/padthv1_wave.h
  ${PADTHV1_SRC_DIR}/padthv1_ramp.h
  ${PADTHV1_SRC_DIR}/padthv1_list.h
  ${PADTHV1_SRC_DIR}/padthv1_fx.h
  ${PADTHV1_SRC_DIR}/padthv1_reverb.h
  padthv1_param.h    # Overridden by my version
  ${PADTHV1_SRC_DIR}/padthv1_sched.h
  ${PADTHV1_SRC_DIR}/padthv1_tuning.h
  ${PADTHV1_SRC_DIR}/padthv1_programs.h
  ${PADTHV1_SRC_DIR}/padthv1_controls.h
)

set (SOURCES
  ${PADTHV1_SRC_DIR}/padthv1.cpp
  ${PADTHV1_SRC_DIR}/padthv1_sample.cpp
  ${PADTHV1_SRC_DIR}/padthv1_config.cpp
  ${PADTHV1_SRC_DIR}/padthv1_formant.cpp
  ${PADTHV1_SRC_DIR}/padthv1_wave.cpp
  padthv1_param.cpp    # Overridden by my version
  ${PADTHV1_SRC_DIR}/padthv1_sched.cpp
  ${PADTHV1_SRC_DIR}/padthv1_tuning.cpp
  ${PADTHV1_SRC_DIR}/padthv1_programs.cpp
  ${PADTHV1_SRC_DIR}/padthv1_controls.cpp
)


set (HEADERS_UI
  ${PADTHV1_SRC_DIR}/padthv1_ui.h
  ${PADTHV1_SRC_DIR}/padthv1widget.h
  ${PADTHV1_SRC_DIR}/padthv1widget_env.h
  ${PADTHV1_SRC_DIR}/padthv1widget_filt.h
  ${PADTHV1_SRC_DIR}/padthv1widget_sample.h
  ${PADTHV1_SRC_DIR}/padthv1widget_wave.h
  ${PADTHV1_SRC_DIR}/padthv1widget_param.h
  ${PADTHV1_SRC_DIR}/padthv1widget_keybd.h
  ${PADTHV1_SRC_DIR}/padthv1widget_preset.h
  ${PADTHV1_SRC_DIR}/padthv1widget_status.h
  ${PADTHV1_SRC_DIR}/padthv1widget_programs.h
  ${PADTHV1_SRC_DIR}/padthv1widget_controls.h
  ${PADTHV1_SRC_DIR}/padthv1widget_control.h
  ${PADTHV1_SRC_DIR}/padthv1widget_palette.h
  ${PADTHV1_SRC_DIR}/padthv1widget_config.h
)

set (SOURCES_UI
  ${PADTHV1_SRC_DIR}/padthv1_ui.cpp
  ${PADTHV1_SRC_DIR}/padthv1widget.cpp
  ${PADTHV1_SRC_DIR}/padthv1widget_env.cpp
  ${PADTHV1_SRC_DIR}/padthv1widget_filt.cpp
  ${PADTHV1_SRC_DIR}/padthv1widget_sample.cpp
  ${PADTHV1_SRC_DIR}/padthv1widget_wave.cpp
  ${PADTHV1_SRC_DIR}/padthv1widget_param.cpp
  ${PADTHV1_SRC_DIR}/padthv1widget_keybd.cpp
  ${PADTHV1_SRC_DIR}/padthv1widget_preset.cpp
  ${PADTHV1_SRC_DIR}/padthv1widget_status.cpp
  ${PADTHV1_SRC_DIR}/padthv1widget_programs.cpp
  ${PADTHV1_SRC_DIR}/padthv1widget_controls.cpp
  ${PADTHV1_SRC_DIR}/padthv1widget_control.cpp
  ${PADTHV1_SRC_DIR}/padthv1widget_palette.cpp
  ${PADTHV1_SRC_DIR}/padthv1widget_config.cpp
)

set (FORMS_UI
  ${PADTHV1_SRC_DIR}/padthv1widget.ui
  ${PADTHV1_SRC_DIR}/padthv1widget_control.ui
  ${PADTHV1_SRC_DIR}/padthv1widget_palette.ui
  ${PADTHV1_SRC_DIR}/padthv1widget_config.ui
)

set (RESOURCES_UI
  ${PADTHV1_SRC_DIR}/padthv1.qrc
)


add_library (${PROJECT_NAME} STATIC
  ${HEADERS}
  ${SOURCES}
)

add_library (${PROJECT_NAME}_ui STATIC
  ${HEADERS_UI}
  ${SOURCES_UI}
  ${FORMS_UI}
  ${RESOURCES_UI}
)

if (CONFIG_DPF)
  set (DPF_PLUGIN_TYPES)
  if (CONFIG_DPF_VST2)
    list (APPEND DPF_PLUGIN_TYPES "vst2")
  endif ()
  if (CONFIG_DPF_VST3)
    list (APPEND DPF_PLUGIN_TYPES "vst3")
  endif ()
  if (CONFIG_DPF_CLAP)
    list (APPEND DPF_PLUGIN_TYPES "clap")
  endif ()
  if (CONFIG_DPF_LV2)
    list (APPEND DPF_PLUGIN_TYPES "lv2")
  endif ()

  dpf_add_plugin (${PROJECT_NAME}_dpf
    TARGETS ${DPF_PLUGIN_TYPES}
    UI_TYPE external
    FILES_DSP
      padthv1_dpf.cpp
    FILES_UI
      padthv1_dpfui.cpp
      padthv1widget_dpf.cpp
    MONOLITHIC
  )
endif ()

set_target_properties (${PROJECT_NAME}    PROPERTIES CXX_STANDARD 17)
set_target_properties (${PROJECT_NAME}_ui PROPERTIES CXX_STANDARD 17)

if (QT_VERSION VERSION_GREATER_EQUAL 6.0.0)
  target_compile_options (${PROJECT_NAME}    PRIVATE -fPIC)
  target_compile_options (${PROJECT_NAME}_ui PRIVATE -fPIC)
endif ()

# Add some debugger flags.
if (CONFIG_DEBUG AND UNIX AND NOT APPLE)
  set (CONFIG_DEBUG_OPTIONS -g -fsanitize=address -fno-omit-frame-pointer)
# target_compile_options (${PROJECT_NAME} PRIVATE ${CONFIG_DEBUG_OPTIONS})
# target_compile_options (${PROJECT_NAME}_ui PRIVATE ${CONFIG_DEBUG_OPTIONS})
endif ()

target_link_libraries (${PROJECT_NAME}    PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Xml)
target_link_libraries (${PROJECT_NAME}_ui PUBLIC Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Svg ${PROJECT_NAME})

if (CONFIG_FFTW3)
  if (CONFIG_BUILD_FFTW3)
    target_link_libraries (${PROJECT_NAME} PRIVATE fftw3f)
  else ()
    target_link_libraries (${PROJECT_NAME} PRIVATE PkgConfig::FFTW3)
  endif ()
endif ()

if (CONFIG_DPF)
  set_target_properties (${PROJECT_NAME}_dpf PROPERTIES CXX_STANDARD 17)
  set_target_properties (${PROJECT_NAME}_dpf-dsp PROPERTIES CXX_STANDARD 17)
  set_target_properties (${PROJECT_NAME}_dpf-ui PROPERTIES CXX_STANDARD 17)

  target_link_libraries (${PROJECT_NAME}_dpf PRIVATE ${PROJECT_NAME}_ui)

  target_include_directories (${PROJECT_NAME}_dpf PRIVATE ${Qt6Widgets_INCLUDE_DIRS})
  target_include_directories (${PROJECT_NAME}_dpf-dsp PRIVATE ${Qt6Widgets_INCLUDE_DIRS})
  target_include_directories(${PROJECT_NAME}_dpf-ui PRIVATE ${Qt6Widgets_INCLUDE_DIRS})

  if (CONFIG_FFTW3 AND CONFIG_BUILD_FFTW3)
    # Must let DPF plugin build depend on FFTW3, otherwise you will encounter error with Ninja.
    # NOTE: Simply add dependency for ${PROJECT_NAME}_dpf is not enough. Should add for "dsp" and "ui" subtarget as well.
    add_dependencies (${PROJECT_NAME}_dpf dep_fftw3)
    add_dependencies (${PROJECT_NAME}_dpf-dsp dep_fftw3)
    add_dependencies (${PROJECT_NAME}_dpf-ui dep_fftw3)

    target_link_libraries (${PROJECT_NAME}_dpf PRIVATE fftw3f)
  endif ()

  set (DPF_BINARY_DIR ${PROJECT_BINARY_DIR}/bin)

  if (UNIX AND NOT APPLE)
    if (CONFIG_PREFIX MATCHES $ENV{HOME})
      set (CONFIG_VSTDIR ${CONFIG_PREFIX}/.vst)
      set (CONFIG_VST3DIR ${CONFIG_PREFIX}/.vst3)
      set (CONFIG_CLAPDIR ${CONFIG_PREFIX}/.clap)
      set (CONFIG_LV2DIR ${CONFIG_PREFIX}/.lv2)
    else ()
      set (CONFIG_VSTDIR ${CONFIG_LIBDIR}/vst)
      set (CONFIG_VST3DIR ${CONFIG_LIBDIR}/vst3)
      set (CONFIG_CLAPDIR ${CONFIG_LIBDIR}/clap)
      set (CONFIG_LV2DIR ${CONFIG_LIBDIR}/lv2)
    endif ()

    set (DPF_VST2_BINARY ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf-vst2.so)
    set (DPF_CLAP_BINARY ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf.clap)
    set (DPF_LV2_BINARY ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf.lv2/${PROJECT_NAME}_dpf.so)
    dpf__determine_vst3_package_architecture(VST3_ARCH)
    set (DPF_VST3_BINARY ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf.vst3/Contents/${VST3_ARCH}-linux/${PROJECT_NAME}_dpf.so)

    set (VST2_SUFFIX .so)
    set (VST3_SUFFIX .so)
  elseif (WIN32)
    set (CONFIG_VSTDIR ${CONFIG_WINDOWS_VST_PATH})
    set (CONFIG_VST3DIR ${CONFIG_WINDOWS_VST3_PATH})
    set (CONFIG_CLAPDIR ${CONFIG_WINDOWS_CLAP_PATH})
    set (CONFIG_LV2DIR ${CONFIG_WINDOWS_LV2_PATH})

    set (DPF_VST2_BINARY ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf-vst2.dll)
    set (DPF_CLAP_BINARY ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf.clap)
    set (DPF_LV2_BINARY ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf.lv2/${PROJECT_NAME}_dpf.dll)
    dpf__determine_vst3_package_architecture(VST3_ARCH)
    set (DPF_VST3_BINARY ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf.vst3/Contents/${VST3_ARCH}-win/${PROJECT_NAME}_dpf.vst3)

    set (VST2_SUFFIX .dll)
    set (VST3_SUFFIX .vst3)
  endif ()

  if (NOT CONFIG_DEBUG)
    if (CONFIG_DPF_VST2)
      add_custom_command(TARGET ${PROJECT_NAME}_dpf-vst2  POST_BUILD
        COMMAND strip ${DPF_VST2_BINARY})
    endif ()
    if (CONFIG_DPF_VST3)
      add_custom_command(TARGET ${PROJECT_NAME}_dpf-vst3  POST_BUILD
        COMMAND strip ${DPF_VST3_BINARY})
    endif ()
    if (CONFIG_DPF_CLAP)
      add_custom_command(TARGET ${PROJECT_NAME}_dpf-clap  POST_BUILD
        COMMAND strip ${DPF_CLAP_BINARY})
    endif ()
    if (CONFIG_DPF_LV2)
      add_custom_command(TARGET ${PROJECT_NAME}_dpf-lv2  POST_BUILD
        COMMAND strip ${DPF_LV2_BINARY})
    endif ()
  endif ()

  if (CONFIG_DPF_VST2)
    install (FILES ${DPF_VST2_BINARY}
      RENAME ${PROJECT_NAME}-vst2${VST2_SUFFIX}
      DESTINATION ${CONFIG_VSTDIR})
  endif ()

  if (CONFIG_DPF_VST3)
    install (FILES ${DPF_VST3_BINARY}
      RENAME ${PROJECT_NAME}${VST3_SUFFIX}
      DESTINATION ${CONFIG_VST3DIR}/${PROJECT_NAME}.vst3/Contents/${VST3_ARCH}-linux)
  endif ()

  if (CONFIG_DPF_CLAP)
    install (FILES ${DPF_CLAP_BINARY}
      RENAME ${PROJECT_NAME}.clap
      DESTINATION ${CONFIG_CLAPDIR})
  endif ()

  if (CONFIG_DPF_LV2)   # Explicitly separated from the default LV2 build
    install (FILES ${DPF_LV2_BINARY}
      ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf.lv2/manifest.ttl
      ${DPF_BINARY_DIR}/${PROJECT_NAME}_dpf.lv2/${PROJECT_NAME}_dpf.ttl
      DESTINATION ${CONFIG_LV2DIR}/${PROJECT_NAME}_dpf.lv2)
  endif ()
endif ()
