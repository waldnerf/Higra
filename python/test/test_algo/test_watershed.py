############################################################################
# Copyright ESIEE Paris (2018)                                             #
#                                                                          #
# Contributor(s) : Benjamin Perret                                         #
#                                                                          #
# Distributed under the terms of the CECILL-B License.                     #
#                                                                          #
# The full license is in the file LICENSE, distributed with this software. #
############################################################################

import unittest
import numpy as np
import higra as hg


class TestWatershed(unittest.TestCase):

    def test_watershed(self):
        g = hg.get_4_adjacency_graph((4, 4))
        edge_weights = np.asarray((1, 2, 5, 5, 5, 8, 1, 4, 3, 4, 4, 1, 5, 2, 6, 3, 5, 4, 0, 7, 0, 3, 4, 0))

        labels = hg.labelisation_watershed(edge_weights, g)
        expected = ((1, 1, 1, 2),
                    (1, 1, 2, 2),
                    (1, 1, 3, 3),
                    (1, 1, 3, 3))
        self.assertTrue(np.allclose(labels, expected))


if __name__ == '__main__':
    unittest.main()