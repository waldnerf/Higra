//
// Created by perretb on 02/05/18.
//

#include "py_tree_accumulators.hpp"
#include "py_common_graph.hpp"
#include "xtensor-python/pyarray.hpp"
#include "xtensor-python/pytensor.hpp"
#include "higra/accumulator/tree_accumulator.hpp"

namespace py = pybind11;

using graph_t = hg::tree;
using edge_t = graph_t::edge_descriptor;
using vertex_t = graph_t::vertex_descriptor;


template<typename graph_t>
struct def_accumulate_parallel {
    template<typename value_t, typename C>
    static
    void def(C &c, const char *doc) {
        c.def("accumulate_parallel", [](const graph_t &tree, const xt::pyarray <value_t> &input,
                                        hg::accumulators accumulator) {
                  switch (accumulator) {
                      case hg::accumulators::min:
                          return hg::accumulate_parallel(tree, input, hg::accumulator_min<value_t>());
                          break;
                      case hg::accumulators::max:
                          return hg::accumulate_parallel(tree, input, hg::accumulator_max<value_t>());
                          break;
                      case hg::accumulators::mean:
                          return hg::accumulate_parallel(tree, input, hg::accumulator_mean<value_t>());
                          break;
                      case hg::accumulators::counter:
                          return hg::accumulate_parallel(tree, input, hg::accumulator_counter());
                          break;
                      case hg::accumulators::sum:
                          return hg::accumulate_parallel(tree, input, hg::accumulator_sum<value_t>());
                          break;
                      case hg::accumulators::prod:
                          return hg::accumulate_parallel(tree, input, hg::accumulator_prod<value_t>());
                          break;
                  }
                  throw std::runtime_error("Unknown accumulator.");
              },
              doc,
              py::arg("tree"),
              py::arg("input"),
              py::arg("accumulator"));
    }
};

template<typename graph_t>
struct def_accumulate_sequential {
    template<typename value_t, typename C>
    static
    void def(C &c, const char *doc) {
        c.def("accumulate_sequential",
              [](const graph_t &tree, const xt::pyarray <value_t> &vertex_data, hg::accumulators accumulator) {
                  switch (accumulator) {
                      case hg::accumulators::min:
                          return hg::accumulate_sequential(tree, vertex_data, hg::accumulator_min<value_t>());
                          break;
                      case hg::accumulators::max:
                          return hg::accumulate_sequential(tree, vertex_data, hg::accumulator_max<value_t>());
                          break;
                      case hg::accumulators::mean:
                          return hg::accumulate_sequential(tree, vertex_data, hg::accumulator_mean<value_t>());
                          break;
                      case hg::accumulators::counter:
                          return hg::accumulate_sequential(tree, vertex_data, hg::accumulator_counter());
                          break;
                      case hg::accumulators::sum:
                          return hg::accumulate_sequential(tree, vertex_data, hg::accumulator_sum<value_t>());
                          break;
                      case hg::accumulators::prod:
                          return hg::accumulate_sequential(tree, vertex_data, hg::accumulator_prod<value_t>());
                          break;
                  }
                  throw std::runtime_error("Unknown accumulator.");
              },
              doc,
              py::arg("tree"),
              py::arg("leaf_data"),
              py::arg("accumulator"));
    }
};


struct functorMax {
    template<typename T1, typename T2>
    auto operator()(T1 &&a, T2 &&b) {
        return xt::maximum(std::forward<T1>(a), std::forward<T2>(b));
    }
};

struct functorMin {
    template<typename T1, typename T2>
    auto operator()(T1 &&a, T2 &&b) {
        return xt::minimum(std::forward<T1>(a), std::forward<T2>(b));
    }
};

struct functorPlus {
    template<typename T1, typename T2>
    auto operator()(T1 &&a, T2 &&b) {
        return a + b;
    }
};

struct functorMultiply {
    template<typename T1, typename T2>
    auto operator()(T1 &&a, T2 &&b) {
        return a * b;
    }
};


template<typename graph_t>
struct def_accumulate_and_combine_sequential {
    template<typename value_t, typename C, typename F>
    static
    void def(C &c, const char *doc, const char *name, const F &f) {
        c.def(name,
              [&f](const graph_t &tree, const xt::pyarray <value_t> &input, const xt::pyarray <value_t> &vertex_data,
                   hg::accumulators accumulator) {
                  switch (accumulator) {
                      case hg::accumulators::min:
                          return hg::accumulate_and_combine_sequential(tree, input, vertex_data,
                                                                       hg::accumulator_min<value_t>(),
                                                                       f);
                          break;
                      case hg::accumulators::max:
                          return hg::accumulate_and_combine_sequential(tree, input, vertex_data,
                                                                       hg::accumulator_max<value_t>(),
                                                                       f);
                          break;
                      case hg::accumulators::mean:
                          return hg::accumulate_and_combine_sequential(tree, input, vertex_data,
                                                                       hg::accumulator_mean<value_t>(),
                                                                       f);
                          break;
                      case hg::accumulators::counter:
                          return hg::accumulate_and_combine_sequential(tree, input, vertex_data,
                                                                       hg::accumulator_counter(),
                                                                       f);
                          break;
                      case hg::accumulators::sum:
                          return hg::accumulate_and_combine_sequential(tree, input, vertex_data,
                                                                       hg::accumulator_sum<value_t>(),
                                                                       f);
                          break;
                      case hg::accumulators::prod:
                          return hg::accumulate_and_combine_sequential(tree, input, vertex_data,
                                                                       hg::accumulator_prod<value_t>(),
                                                                       f);
                          break;
                  }
                  throw std::runtime_error("Unknown accumulator.");
              },
              doc,
              py::arg("tree"),
              py::arg("input"),
              py::arg("leaf_data"),
              py::arg("accumulator"));
    }
};


template<typename graph_t>
struct def_propagate_sequential {
    template<typename value_t, typename C>
    static
    void def(C &c, const char *doc) {
        c.def("propagate_sequential",
              [](const graph_t &tree, const xt::pyarray <value_t> &input,
                 const xt::pyarray<bool> &condition) {
                  return hg::propagate_sequential(tree, input, condition);
              },
              doc,
              py::arg("tree"),
              py::arg("input"),
              py::arg("condition"));
    }
};

template<typename graph_t>
struct def_propagate_parallel {
    template<typename value_t, typename C>
    static
    void def(C &c, const char *doc) {
        c.def("propagate_parallel",
              [](const graph_t &tree, const xt::pyarray <value_t> &input,
                 const xt::pyarray<bool> &condition) {
                  return hg::propagate_parallel(tree, input, condition);
              },
              doc,
              py::arg("tree"),
              py::arg("input"),
              py::arg("condition"));
    }
};

void py_init_tree_accumulator(pybind11::module &m) {
    add_type_overloads<def_accumulate_parallel<graph_t>, HG_TEMPLATE_NUMERIC_TYPES>
            (m,
             "For each node i of the tree, we accumulate values of the children of i in the input array and put the result "
             "in output. i.e. output(i) = accumulate(input(children(i)))");

    add_type_overloads<def_accumulate_sequential<graph_t>, HG_TEMPLATE_NUMERIC_TYPES>
            (m,
             "Performs a sequential accumulation of node values from the leaves to the root. "
             "For each leaf node i, output(i) = leaf_data(i)."
             "For each node i from the leaves (excluded) to the root, output(i) = accumulate(output(children(i)))");

    add_type_overloads<def_accumulate_and_combine_sequential<graph_t>, HG_TEMPLATE_NUMERIC_TYPES>
            (m,
             "Performs a sequential accumulation of node values from the leaves to the root and add the result with the input array."
             "For each leaf node i, output(i) = leaf_data(i)."
             "For each node i from the leaves (excluded) to the root, output(i) = input(i) + accumulate(output(children(i)))",
             "accumulate_and_add_sequential",
             functorPlus());

    add_type_overloads<def_accumulate_and_combine_sequential<graph_t>, HG_TEMPLATE_NUMERIC_TYPES>
            (m,
             "Performs a sequential accumulation of node values from the leaves to the root and multiply the result with the input array."
             "For each leaf node i, output(i) = leaf_data(i)."
             "For each node i from the leaves (excluded) to the root, output(i) = input(i) * accumulate(output(children(i)))",
             "accumulate_and_multiply_sequential",
             functorMultiply());

    add_type_overloads<def_accumulate_and_combine_sequential<graph_t>, HG_TEMPLATE_NUMERIC_TYPES>
            (m,
             "Performs a sequential accumulation of node values from the leaves to the root and max the result with the input array."
             "For each leaf node i, output(i) = leaf_data(i)."
             "For each node i from the leaves (excluded) to the root, output(i) = max(input(i), accumulate(output(children(i))))",
             "accumulate_and_max_sequential",
             functorMax());

    add_type_overloads<def_accumulate_and_combine_sequential<graph_t>, HG_TEMPLATE_NUMERIC_TYPES>
            (m,
             "Performs a sequential accumulation of node values from the leaves to the root and min the result with the input array."
             "For each leaf node i, output(i) = leaf_data(i)."
             "For each node i from the leaves (excluded) to the root, output(i) = min(input(i), accumulate(output(children(i))))",
             "accumulate_and_min_sequential",
             functorMin());

    add_type_overloads<def_propagate_parallel<graph_t>, HG_TEMPLATE_NUMERIC_TYPES>
            (m,
             "Conditionally propagates parent values to children. "
             "For each node i, if condition(i) then output(i) = input(tree.parent(i)) otherwise"
             "output(i) = input(i)");

    add_type_overloads<def_propagate_sequential<graph_t>, HG_TEMPLATE_NUMERIC_TYPES>
            (m,
             "Conditionally propagates parent values to children. "
             "For each node i from the root to the leaves, if condition(i) then output(i) = output(tree.parent(i)) otherwise"
             "output(i) = input(i)");
}