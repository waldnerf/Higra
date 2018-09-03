############################################################################
# Copyright ESIEE Paris (2018)                                             #
#                                                                          #
# Contributor(s) : Benjamin Perret                                         #
#                                                                          #
# Distributed under the terms of the CECILL-B License.                     #
#                                                                          #
# The full license is in the file LICENSE, distributed with this software. #
############################################################################

import higra as hg


@hg.data_consumer("edge_weights")
def bpt_canonical(graph, edge_weights):
    """
    Compute the canonical binary partition tree (binary tree by altitude ordering) of the given weighted graph.


    :param graph:
    :param edge_weights:
    :return: Tree (with attributes "leaf_graph", "altitudes" and "mst")
    """

    tree, altitudes, mst = hg._bpt_canonical(graph, edge_weights)

    original_graph = hg.get_attribute(graph, "original_graph")
    if original_graph:
        hg.set_attribute(tree, "leaf_graph", original_graph)
        hg.set_attribute(mst, "original_graph", original_graph)
    else:
        hg.set_attribute(tree, "leaf_graph", graph)
        hg.set_attribute(mst, "original_graph", graph)

    hg.set_attribute(tree, "altitudes", altitudes)
    hg.set_attribute(tree, "mst", mst)

    return tree


@hg.data_consumer("deleted_vertices")
def simplify_tree(tree, deleted_vertices):
    """
    Creates a copy of the current Tree and deletes the vertices i such that deletedVertices[i] is true.

    The attribute "node_map" of the returned tree is an array that maps any node index i of the new tree,
    to the index of this node in the original tree.

    :param tree:
    :param deleted_vertices:
    :return: simplified tree (with attributes "node_map")
    """

    new_tree, node_map = hg._simplify_tree(tree, deleted_vertices)

    hg.set_attribute(new_tree, "leaf_graph", hg.getAttribute(tree, "leaf_graph"))
    hg.set_attribute(new_tree, "node_map", node_map)

    return new_tree


@hg.data_consumer("altitudes", "lca_map")
def saliency(tree, altitudes, lca_map):
    """
    Compute the saliency map of the given tree
    :param tree:
    :param altitudes: altitudes of the vertices of the tree
    :param lca_map: array containing the lowest common ancestor of the source and target vertices of each edge
    where saliency need to be computed
    :return: altitudes[lca_map]
    """

    return altitudes[lca_map]


@hg.data_consumer("altitudes")
def compute_bpt_merge_attribute(tree, attribute, altitudes):
    """
    In the context of watershed hierarchy by attributes, computes for each node n,
    the level at which n disappears according to the given attribute of the canonical
    binary partition tree.

    :param tree: canonical binary partition tree of an edge-weighted graph
    :param attribute: raw attribute computed on the bpt
    :param altitudes: altitudes of the nodes of the bpt
    :return: array giving the level of disappearance of each node of the bpt
    """
    non_qfz_nodes = altitudes == altitudes[tree.parents()]
    non_qfz_nodes[-1] = False
    qfz_nodes_altitudes = attribute.copy()
    # TODO generalize for possibly negative attributes !
    qfz_nodes_altitudes[non_qfz_nodes] = 0
    extinction = hg.accumulate_and_max_sequential(tree, qfz_nodes_altitudes, attribute[:tree.num_leaves()],
                                                  hg.Accumulators.max)
    persistence = hg.accumulate_parallel(tree, extinction, hg.Accumulators.min)
    persistence[:tree.num_leaves()] = 0
    return persistence


@hg.data_consumer("edge_weights")
def binary_partition_tree_complete_linkage(graph, edge_weights):
    """
    Compute a binary partition tree with complete linkage distance.

    Given a graph G, with initial edge weights W,
    the distance d(X,Y) between any two regions X, Y is defined as :\n
    d(X,Y) = max {W({x,y}) | x in X, y in Y, {x,y} in G }

    :param graph:
    :param edge_weights:
    :return:
    """

    tree, altitudes = hg._binary_partition_tree_complete_linking(graph, edge_weights)

    hg.set_attribute(tree, "leaf_graph", graph)
    hg.set_attribute(tree, "altitudes", altitudes)

    return tree


def binary_partition_tree_average_linkage(graph, edge_values, edge_weights):
    """
    Compute a binary partition tree with average linkage distance.

    Given a graph G, with initial edge values V with associated weights W,
    the distance d(X,Y) between any two regions X, Y is defined as:
    d(X,Y) = (1 / Z) + sum_{x in X, y in Y, {x,y} in G} V({x,y}) x W({x,y})
    with Z = sum_{x in X, y in Y, {x,y} in G} W({x,y})

    :param graph:
    :param edge_values:
    :param edge_weights:
    :return:
    """

    tree, altitudes = hg._binary_partition_tree_average_linking(graph, edge_values,  edge_weights)

    hg.set_attribute(tree, "leaf_graph", graph)
    hg.set_attribute(tree, "altitudes", altitudes)

    return tree


@hg.data_consumer("edge_weights")
def binary_partition_tree_single_linkage(graph, edge_weights):
    """
    Alias for bpt_canonical

    :param graph:
    :param edge_weights:
    :return:
    """

    return bpt_canonical(graph, edge_weights)