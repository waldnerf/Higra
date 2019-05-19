/***************************************************************************
* Copyright ESIEE Paris (2018)                                             *
*                                                                          *
* Contributor(s) : Benjamin Perret                                         *
*                                                                          *
* Distributed under the terms of the CECILL-B License.                     *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#pragma once

#include <stdio.h>
#include <exception>
#include <string>
#include <iostream>
#include <stack>
#include "xtensor/xstrided_view.hpp"
#include "xtensor/xio.hpp"
#include "detail/log.hpp"

namespace hg {

    /**
     * Preferred type to represent an index
     */
    using index_t = std::ptrdiff_t;

    /**
     * Constant used to represent an invalid index (eg. not initialized)
     */
    const index_t invalid_index = -1;

    /**
     * Preferred type to represent a size
     */
    using size_t = std::size_t;
}

#define HG_MAIN_ASSERT

#ifndef __FUNCTION_NAME__
#ifdef WIN32   //WINDOWS
#define __FUNCTION_NAME__   __FUNCTION__
#else          //*NIX
#define __FUNCTION_NAME__   __func__
#endif
#endif

#ifdef HG_MAIN_ASSERT
#define hg_assert(test, msg) do { \
    if(!(test)) {\
    throw std::runtime_error(std::string() + __FUNCTION_NAME__ + " in file " + __FILE__ + "(line:" + std::to_string(__LINE__) + "): "  + msg);} \
  } while (0)

#define hg_assert_edge_weights(graph, edge_weights) do { \
    hg_assert(edge_weights.dimension() > 0, \
              "The dimension of the array '" #edge_weights "', representing edge data of the graph '" #graph "' must be at least 1.");\
    hg_assert(num_edges(graph) == edge_weights.shape()[0], "The dimension of the provided edge data array '" #edge_weights "' does "\
                                                           "not match the number of edges in the provided graph '" #graph "'.");\
    } while (0)

#define hg_assert_vertex_weights(graph, vertex_weights) do { \
    hg_assert(vertex_weights.dimension() > 0, \
              "The dimension of the array '" #vertex_weights "', representing vertex data of the graph '" #graph "' must be at least 1.");\
    hg_assert(num_vertices(graph) == vertex_weights.shape()[0], "The dimension of the provided vertex data array '" #vertex_weights "' does "\
                                                           "not match the number of vertices in the provided graph '" #graph "'.");\
    } while (0)

#define hg_assert_node_weights(tree, node_weights) do { \
    hg_assert(node_weights.dimension() > 0, \
              "The dimension of the array '" #node_weights "', representing node data of the tree '" #tree "' must be at least 1.");\
    hg_assert(num_vertices(tree) == node_weights.shape()[0], "The dimension of the provided node data array '" #node_weights "' does "\
                                                           "not match the number of nodes in the provided tree '" #tree "'.");\
    } while (0)

#define hg_assert_leaf_weights(tree, leaf_weights) do { \
    hg_assert(leaf_weights.dimension() > 0, \
              "The dimension of the array '" #leaf_weights "', representing leaves data of the tree '" #tree "' must be at least 1.");\
    hg_assert(num_leaves(tree) == leaf_weights.shape()[0], "The dimension of the provided leaf data array '" #leaf_weights "' does "\
                                                           "not match the number of leaves in the provided tree '" #tree "'.");\
    } while (0)

#define hg_assert_1d_array(array) do { \
    hg_assert(array.dimension() == 1, "The array '" #array "' must be 1d."); \
    } while (0)

#define hg_assert_integral_value_type(array) do { \
    static_assert(std::is_integral<typename std::decay_t<decltype(array)>::value_type>::value, "Array values of '" #array "' must be integral (char, short, int, long...)."); \
    } while (0)

#define hg_assert_same_shape(array1, array2) do { \
    hg_assert(xt::same_shape(array1.shape(), array2.shape()), "Shapes of '" #array1 "' and '" #array2 "' must be equal."); \
    } while (0)

#define hg_assert_component_tree(tree) do { \
    hg_assert(hg::category(tree) == hg::tree_category::component_tree, "The category of '" #tree "' must be 'component_tree'."); \
    } while (0)

#define hg_assert_partition_tree(tree) do { \
    hg_assert(hg::category(tree) == hg::tree_category::partition_tree, "The category of '" #tree "' must be 'partition_tree'."); \
    } while (0)
#else
#define hg_assert(test, msg) ((void)0)
#define hg_assert_vertex_weights(graph, vertex_weights) ((void)0)
#define hg_assert_edge_weights(graph, vertex_weights) ((void)0)
#define hg_assert_node_weights(tree, node_weights) ((void)0)
#define hg_assert_leaf_weights(tree, leaf_weights) ((void)0)
#define hg_assert_1d_array(array) ((void)0)
#define hg_assert_integral_value_type(array) ((void)0)
#define hg_assert_same_shape(array1, array2) ((void)0)
#define hg_assert_component_tree(tree) ((void)0)
#define hg_assert_partition_tree(tree) ((void)0)
#endif

// Check windows
#if _WIN32 || _WIN64
#define HG_INT_64 long long
#define HG_UINT_64 unsigned long long
#elif __GNUC__ || __clang__
#if __x86_64__ || __ppc64__
#define HG_INT_64 long
#define HG_UINT_64 unsigned long
#else
#define HG_INT_64 long long
#define HG_UINT_64 unsigned long long
#endif
#else
#if !defined(HG_INT_64) && !defined(HG_UINT_64)
#error Unkown architecture, please provide the two macros HG_INT_64 and HG_UINT_64 defining respectively the types of 64 integers and unsigned 64 bits integers
#endif
#endif


#define HG_XSTR(a) HG_STR(a)
#define HG_STR(a) #a


#define HG_TEMPLATE_SINTEGRAL_TYPES   char, short, int, HG_INT_64

#define HG_TEMPLATE_INTEGRAL_TYPES    char, unsigned char, short, unsigned short, int, unsigned int, HG_INT_64, HG_UINT_64

#define HG_TEMPLATE_FLOAT_TYPES       float, double

#define HG_TEMPLATE_NUMERIC_TYPES     char, unsigned char, short, unsigned short, int, unsigned int, HG_INT_64, HG_UINT_64, float, double

#define HG_TEMPLATE_SNUMERIC_TYPES    char, short, int, HG_INT_64, float, double


namespace xt {

    inline
    bool all(bool x) {
        return x;
    }

    inline
    bool any(bool x) {
        return x;
    }

    /**
     * Provides a view semantic over any xtensor container
     * @tparam E
     * @param e
     * @return
     */
    template<typename E,
            typename = std::enable_if_t<!std::is_base_of<xt::xexpression<E>, E>::value> >
    E &&view_all(E &&e) {
        return std::forward<E>(e);
    }

    /**
     * Provides a view semantic over any xtensor container
     * @tparam E
     * @param e
     * @return
     */
    template<typename E>
    auto view_all(xt::xcontainer_semantic<E> &&e) {
        return xt::strided_view(e, {});
    }

    /**
     * Provides a view semantic over any xtensor container
     * @tparam E
     * @param e
     * @return
     */
    template<typename E>
    auto &&view_all(xt::xview_semantic<E> &&e) {
        return std::forward<xt::xview_semantic<E>>(e);
    }

    /**
     * Provides a view semantic over any xtensor container
     * @tparam E
     * @param e
     * @return
     */
    template<typename E>
    auto view_all(xt::xscalar<E> &&e) {
        return xt::strided_view(e, {});
    }

}

namespace hg {

    /**
     * Insert all elements of collection b at the end of collection a.
     * @tparam T1 must have an insert method (STL like) and a range interface (begin, end)
     * @tparam T2 must have a range interface (begin, end)
     * @param a
     * @param b
     */
    template<typename T1, typename T2>
    void extend(T1 &a, const T2 &b) {
        a.insert(std::end(a), std::begin(b), std::end(b));
    };

    template<typename T>
    using stackv = std::stack<T, std::vector<T>>;

    /**
     * Do not use except if you want a compile error showing the type of the provided template parameter !
     * @tparam T
     */
    template<typename T>
    struct COMPILE_ERROR;

}


