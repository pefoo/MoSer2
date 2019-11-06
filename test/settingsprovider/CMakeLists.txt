set(SOURCES
  main.cpp
  settingsprovider_test.cpp
)

add_executable(settingsprovider_unit_test ${SOURCES})
target_link_libraries(settingsprovider_unit_test SettingsProvider)
add_test(NAME SettingsProvider_Test COMMAND "${CMAKE_BINARY_DIR}/settingsprovider_unit_test")
