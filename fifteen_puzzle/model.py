
"""This module contains class `Model`"""

import logging

class Model:
    """Logic model of fifteen puzzle"""

    def __init__(self, (width, height)):
        self.__width, self.__height = width, height
        # Create tiles
        self.__matrix = [
            [
                i + j * self.__width
                for i in range(self.__width)
            ]
            for j in range(self.__height)
        ]
        # Create hole
        self.__matrix[-1][-1] = None
        self.__hole_pos = (-1, -1)

    def __getitem__(self, (x, y)):
        return self.__matrix[y][x]

    def get_dimension(self):
        return self.__width, self.__height

    def touch_tile(self, (x, y)):
        logging.debug('Model.touch_file(self, %s)' % (str((x, y))))

        # Check position
        self[(x, y)]

        if self.__is_adj((x, y), self.__hole_pos):
            self.__swap((x, y), self.__hole_pos)
            self.__hole_pos = (x, y)
            self.success()
        else:
            self.failure()
        
    def __is_adj(self, (x1, y1), (x2, y2)):
        if x1 < 0:
            x1 += self.__width
        if x2 < 0:
            x2 += self.__width
        if y1 < 0:
            y1 += self.__width
        if y2 < 0:
            y2 += self.__width

        if x1 == x2 and y1 in (y2 - 1, y2 + 1):
            return True
        elif y1 == y2 and x1 in (x2 - 1, x2 + 1):
            return True
        else:
            return False

    def __swap(self, (x1, y1), (x2, y2)):
        tmp = self.__matrix[y1][x1]
        self.__matrix[y1][x1] = self.__matrix[y2][x2]
        self.__matrix[y2][x2] = tmp

    def failure():
        """This method is invoked after failed moves"""
        pass

    def success():
        """This method is invoked after success moves"""
        pass

if __name__ == '__main__':

    import unittest

    class Test(unittest.TestCase):

        def testInit(self):
            instance = Model((4, 4))
            self.assertEqual(0, instance[(0, 0)])
            self.assertEqual(1, instance[(1, 0)])
            self.assertEqual(4, instance[(0, 1)])
            self.assertEqual(None, instance[(-1, -1)])

        def testSwap(self):
            instance = Model((4, 4))
            instance._Model__swap((0, 0), (-1, -1))
            self.assertEqual(0, instance[(-1, -1)])
            self.assertEqual(None, instance[(0, 0)])

        def testSwapSuccessFail(self):
            class SubModel(Model):
                success_count = 0
                failure_count = 0
                def reset(self):
                    SubModel.success_count = 0
                    SubModel.failure_count = 0
                def success(self):
                    SubModel.success_count += 1
                def failure(self):
                    SubModel.failure_count += 1

            instance = SubModel((4, 4))

            instance.touch_tile((0, 0))
            instance.touch_tile((-1, -1))
            instance.touch_tile((-2, -2))
            self.assertEqual(0, SubModel.success_count)
            self.assertEqual(3, SubModel.failure_count)

            instance.reset()
            instance.touch_tile((-1, -2))
            instance.touch_tile((-2, -2))
            instance.touch_tile((2, 3))
            self.assertEqual(3, SubModel.success_count)
            self.assertEqual(0, SubModel.failure_count)

    unittest.main()
