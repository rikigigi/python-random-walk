
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "randomwalk.h"


namespace py = pybind11;

PYBIND11_MODULE(pydif,m) {

    py::class_<RandomWalk>(m,"RandomWalk",py::buffer_protocol())
            .def(py::init<const double & , const double & , const unsigned int &, const size_t & , const size_t & , const uint32_t &>())
            .def("calc",&RandomWalk::calcola)
            .def_buffer([](RandomWalk & m) -> py::buffer_info {
                return py::buffer_info(
                            m.get_data(),
                            sizeof(double),
                            py::format_descriptor<double>::format(),
                            m.get_shape().size(),
                            m.get_shape(),
                            m.get_stride()
                            );
            })
            .def("applyPbc",&RandomWalk::pbc)
            .def("__enter__",[](RandomWalk & m) -> RandomWalk & {return m;})
            .def("__exit__",[](RandomWalk & m, py::object * exc_type, py::object * exc_value, py::object * traceback){})
            ;

}
