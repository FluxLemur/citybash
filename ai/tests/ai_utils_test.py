import sys; sys.path.append('../')
import ai_utils
import unittest

class TestAiUtilParsing(unittest.TestCase):
    def test_parse_world(self):
        string = '\n'.join([
            'WORLD',
            'A (0) 1 9.899495',
            'B (1) 1 6.324555',
            '',
            ])
        out = ai_utils.parse_world(string)

        self.assertEqual(out[0], ('A', 0, 1, 9.899495))
        self.assertEqual(out[1], ('B', 1, 1, 6.324555))

        string = '\n'.join([
            'WORLD',
            'AKJNSDKJAN (120) 2 92.519415',
            'KAJDFN (1) 4 0.0000001',
            'ZZ (20) 4 5.0',
            'AA (3) 1 500.0',
            '',
            ])

        out = ai_utils.parse_world(string)
        self.assertEqual(4, len(out))
        self.assertEqual(out[0], ('AKJNSDKJAN', 120, 2, 92.519415))
        self.assertEqual(out[1], ('KAJDFN', 1, 4, 0.0000001))
        self.assertEqual(out[2], ('ZZ', 20, 4, 5.0))
        self.assertEqual(out[3], ('AA', 3, 1, 500.0))


    def test_parse_city(self):
        string = '\n'.join([
            'CITY A',
            '  LEVEL  1',
            '  GOLD   2',
            '  INCOME 1',
            '  ARMY   0',
            '',

            ])

        level, gold, income, army, notifications = \
            ai_utils.parse_city(string)

        self.assertEqual(1, level)
        self.assertEqual(2, gold)
        self.assertEqual(1, income)
        self.assertEqual(0, army)
        self.assertEqual(0, len(notifications))

        string = '\n'.join([
            'CITY A',
            '  LEVEL  4',
            '  GOLD   1000',
            '  INCOME 10',
            '  ARMY   130',
            '  * 1611 sec ago: B attacked 0/87 took 0 gold, 26/98 defenders remained',
            '  * 1712 sec ago: attacked B 80/80 took 160 gold, 0/0 defenders remained',
            '',
            ])

        level, gold, income, army, notifications = \
            ai_utils.parse_city(string)

        self.assertEqual(4, level)
        self.assertEqual(1000, gold)
        self.assertEqual(10, income)
        self.assertEqual(130, army)
        self.assertEqual(2, len(notifications))
        self.assertEqual((1611, 'attacked by', 'B', 87, 0, 0, 98, 26), notifications[0])
        self.assertEqual((1712, 'attacked', 'B', 80, 80, 160, 0, 0), notifications[1])

        string = '\n'.join([
            'CITY A',
            '  LEVEL  4',
            '  GOLD   1000',
            '  INCOME 10',
            '  ARMY   130',
            '  * 0 sec ago: attacked C 0/1 took 0 gold',
            '  * 1 sec ago: attacked B 0/10 took 0 gold',
            '',
            ])

        level, gold, income, army, notifications = \
            ai_utils.parse_city(string)

        self.assertEqual(4, level)
        self.assertEqual(1000, gold)
        self.assertEqual(10, income)
        self.assertEqual(130, army)
        self.assertEqual(2, len(notifications))
        self.assertEqual((0, 'attacked', 'C', 1, 0, 0, -1, -1), notifications[0])
        self.assertEqual((1, 'attacked', 'B', 10, 0, 0, -1, -1), notifications[1])


    def test_parse_costs(self):
        string = '\n'.join([
            'COSTS',
            'UPGRADE 30 gold, 5 sec',
            'TRAIN   5 gold, 5 sec',
            '',
            ])

        out = ai_utils.parse_costs(string)
        self.assertEqual((30, 5, 5, 5), out)

        string = '\n'.join([
            'COSTS',
            'UPGRADE 300 gold, 100 sec',
            'TRAIN   1 gold, 1 sec',
            '',
            ])

        out = ai_utils.parse_costs(string)
        self.assertEqual((300, 100, 1, 1), out)


if __name__ == '__main__':
    unittest.main()
