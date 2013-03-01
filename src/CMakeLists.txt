set(LIB_NAME "ssd1306")

set(SRC_FILES
  ssd1306.c
  rpiIO.c
)

set(PRIVATE_HEADERS
  rpiIO.h
)

set(PUBLIC_HEADERS
  ../include/ssd1306/ssd1306.h
)



add_library(${LIB_NAME} 
  ${PRIVATE_HDRS}
  ${PUBLIC_HDRS}
  ${SRC_FILES}
)

add_library(${LIB_NAME} SHARED 
  ${PRIVATE_HDRS}
  ${PUBLIC_HDRS}
  ${SRC_FILES}
)

target_link_libraries(${LIB_NAME}
)




set(APP_NAME "testSSD1306")

set(SRC_FILES
  testSSD1306.c
)

set(HDRS
)


include_directories(
   ../include/
)

add_executable(${APP_NAME}
  ${HDRS}
  ${SRC_FILES}
)

target_link_libraries(${APP_NAME}
  ssd1306  
)

if (CMAKE_DEBUG_POSTFIX)
  set_target_properties(${APP_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
endif (CMAKE_DEBUG_POSTFIX)


INSTALL(
  TARGETS ${APP_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  COMPONENT Application
)



set(APP_NAME "testGPIO")

set(SRC_FILES
  testGPIO.c
  rpiIO.c
)

set(HDRS
  rpiIO.h
)


include_directories(
   ../include/
)

add_executable(${APP_NAME}
  ${HDRS}
  ${SRC_FILES}
)

target_link_libraries(${APP_NAME}
  ssd1306  
)

if (CMAKE_DEBUG_POSTFIX)
  set_target_properties(${APP_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
endif (CMAKE_DEBUG_POSTFIX)


INSTALL(
  TARGETS ${APP_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  COMPONENT Application
)

