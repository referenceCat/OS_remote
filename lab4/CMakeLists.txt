add_executable(lab4_task1_reader task1_reader.cpp utils.h)
add_executable(lab4_task1_writer task1_writer.cpp utils.h)
add_executable(lab4_task2_server task2_server.cpp)
add_executable(lab4_task2_client task2_client.cpp)
target_link_libraries(lab4_task1_reader PUBLIC winMM.Lib)
target_link_libraries(lab4_task1_writer PUBLIC winMM.Lib)
add_executable(lab4_task1 task1_dispatcher.cpp utils.h)
target_link_libraries(lab4_task1 PUBLIC winMM.Lib)