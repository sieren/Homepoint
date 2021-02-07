
  set(COMPONENT_SRCDIRS
      "M5Core2/src"
      "M5Core2/src/utility"
  )

  set(COMPONENT_ADD_INCLUDEDIRS
      "M5Core2/src"
      "M5Core2/src/utility"
  )

  set(COMPONENT_REQUIRES
      "arduino-esp32"
  )

  register_component()

  target_compile_definitions(${COMPONENT_TARGET} PUBLIC -DESP32)
  target_compile_definitions(${COMPONENT_TARGET} PUBLIC -DM5Stack_M5Core2)
  # target_compile_definitions(${COMPONENT_TARGET} PUBLIC -DTFT_eSPI_TOUCH_EMULATION)

  target_compile_options(${COMPONENT_TARGET} PRIVATE -fno-rtti)
  target_compile_options(${COMPONENT_TARGET} PRIVATE -Wno-maybe-uninitialized)
  target_compile_options(${COMPONENT_TARGET} PUBLIC -Wno-missing-field-initializers)

