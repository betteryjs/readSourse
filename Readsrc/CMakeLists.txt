add_executable(testList testList.cpp)
add_executable(list_move list_move.cpp)
add_executable(listsplit listsplit.cpp)
#add_executable(testthread testthread.cpp thrdpool.c)
add_executable(testrbTree  testrbTree.cpp rbtree.c)

add_executable(thrdTash thrdTash.cpp )

target_link_libraries(thrdTash workflow)