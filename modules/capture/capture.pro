
SOURCES *= \
    $$PWD/finder/su_abstract_find_field.cpp \       #find field
    $$PWD/finder/su_find_field_on_frame.cpp \
#    $$PWD/finder/noname_algorithm/su_find_field_on_image.cpp \
    $$PWD/finder/noname_algorithm/su_abstract_find_rect.cpp \        #find rect
    $$PWD/finder/noname_algorithm/su_find_rect_la.cpp \
    $$PWD/finder/noname_algorithm/su_abstract_cut_cells.cpp \        #cut cells
    $$PWD/finder/noname_algorithm/su_cut_cells_sample.cpp \
    $$PWD/finder/noname_algorithm/su_cut_cells_smart.cpp \
    $$PWD/finder/noname_algorithm/su_abstract_check_on_field.cpp \   #checking on field
    $$PWD/finder/noname_algorithm/su_check_on_field_none.cpp \
    $$PWD/su_capture_cell.cpp \                     #capture cell
    $$PWD/recognizer/su_abstract_recognizer.cpp \   #recognizer
    $$PWD/recognizer/su_am_recognizer.cpp \
    $$PWD/finder/noname_algorithm/su_noname_finder.cpp


HEADERS *= \
    $$PWD/finder/su_abstract_find_field.h \         #find field
    $$PWD/finder/su_find_field_on_frame.h \
#    $$PWD/finder/noname_algorithm/su_find_field_on_image.h \
    $$PWD/finder/noname_algorithm/su_abstract_find_rect.h \          #find rect
    $$PWD/finder/noname_algorithm/su_find_rect_la.h \
    $$PWD/finder/noname_algorithm/su_abstract_cut_cells.h \          #cut cells
    $$PWD/finder/noname_algorithm/su_cut_cells_sample.h \
    $$PWD/finder/noname_algorithm/su_cut_cells_smart.h \
    $$PWD/finder/noname_algorithm/su_abstract_check_on_field.h \     #checking on field
    $$PWD/finder/noname_algorithm/su_check_on_field_none.h \
    $$PWD/su_algorithm.h \                          #algorithm
    $$PWD/su_capture_cell.h \                       #capture cell
    $$PWD/recognizer/su_abstract_recognizer.h \     #recognizer
    $$PWD/recognizer/su_am_recognizer.h \
    $$PWD/finder/noname_algorithm/su_noname_finder.h \
    modules/capture/recognizer/trainData.h


PATHS *= \
    $$PWD

DEPENDPATH *= $$PATHS

INCLUDEPATH *= $$PATHS

RESOURCES +=
