#include "itemgossiphelp.h"

typedef std::vector<ArenaTeam*> RankingContainer;

	void aprender_habilidad_clase(Player* player)
	{
		switch(player->getClass())
			{
					case CLASS_WARRIOR:	//Warrior Spells Trainer 914
					{
						player->learnSpell(355, false);
						player->learnSpell(33388, false);
						player->learnSpell(2567, false);
						player->learnSpell(33391, false);
						player->learnSpell(34090, false);
						player->learnSpell(34091, false);
						player->learnSpell(54197, false);
						player->learnSpell(7386, false);
						player->learnSpell(674, false);
						player->learnSpell(5011, false);
						player->learnSpell(264, false);
						player->learnSpell(266, false);
						player->learnSpell(200, false);
						player->learnSpell(15590, false);
						player->learnSpell(227, false);
						player->learnSpell(1180, false);
						player->learnSpell(202, false);
						player->learnSpell(201, false);
						player->learnSpell(197, false);
						player->learnSpell(196, false);
						player->learnSpell(3018, false);
						player->learnSpell(199, false);
						player->learnSpell(198, false);
						player->learnSpell(71, false);
						player->learnSpell(2458, false);
						player->learnSpell(12678, false);
						player->learnSpell(25203, false);
						player->learnSpell(25208, false);
						player->learnSpell(25231, false);
						player->learnSpell(25234, false);
						player->learnSpell(25236, false);
						player->learnSpell(25241, false);
						player->learnSpell(25242, false);
						player->learnSpell(25258, false);
						player->learnSpell(25264, false);
						player->learnSpell(25269, false);
						player->learnSpell(25286, false);
						player->learnSpell(25288, false);
						player->learnSpell(25289, false);
						player->learnSpell(25202, false);
						player->learnSpell(23925, false);
						player->learnSpell(23924, false);
						player->learnSpell(18499, false);
						player->learnSpell(20230, false);
						player->learnSpell(20252, false);
						player->learnSpell(20569, false);
						player->learnSpell(20658, false);
						player->learnSpell(20660, false);
						player->learnSpell(20661, false);
						player->learnSpell(20662, false);
						player->learnSpell(23920, false);
						player->learnSpell(23922, false);
						player->learnSpell(23923, false);
						player->learnSpell(29707, false);
						player->learnSpell(47475, false);
						player->learnSpell(47487, false);
						player->learnSpell(47488, false);
						player->learnSpell(47501, false);
						player->learnSpell(47502, false);
						player->learnSpell(47519, false);
						player->learnSpell(47520, false);
						player->learnSpell(55694, false);
						player->learnSpell(57755, false);
						player->learnSpell(57823, false);
						player->learnSpell(47474, false);
						player->learnSpell(47471, false);
						player->learnSpell(47470, false);
						player->learnSpell(30324, false);
						player->learnSpell(30356, false);
						player->learnSpell(30357, false);
						player->learnSpell(34428, false);
						player->learnSpell(46845, false);
						player->learnSpell(47436, false);
						player->learnSpell(47437, false);
						player->learnSpell(47439, false);
						player->learnSpell(47440, false);
						player->learnSpell(47449, false);
						player->learnSpell(47450, false);
						player->learnSpell(47465, false);
						player->learnSpell(64382, false);
						player->learnSpell(11609, false);
						player->learnSpell(72, false);
						player->learnSpell(1719, false);
						player->learnSpell(2048, false);
						player->learnSpell(2565, false);
						player->learnSpell(2687, false);
						player->learnSpell(3127, false);
						player->learnSpell(3411, false);
						player->learnSpell(5242, false);
						player->learnSpell(5246, false);
						player->learnSpell(5308, false);
						player->learnSpell(6178, false);
						player->learnSpell(6190, false);
						player->learnSpell(6192, false);
						player->learnSpell(6343, false);
						player->learnSpell(6546, false);
						player->learnSpell(1715, false);
						player->learnSpell(1680, false);
						player->learnSpell(1608, false);
						player->learnSpell(100, false);
						player->learnSpell(284, false);
						player->learnSpell(285, false);
						player->learnSpell(469, false);
						player->learnSpell(674, false);
						player->learnSpell(676, false);
						player->learnSpell(694, false);
						player->learnSpell(750, false);
						player->learnSpell(772, false);
						player->learnSpell(845, false);
						player->learnSpell(871, false);
						player->learnSpell(1160, false);
						player->learnSpell(1161, false);
						player->learnSpell(1464, false);
						player->learnSpell(6547, false);
						player->learnSpell(6548, false);
						player->learnSpell(11564, false);
						player->learnSpell(11565, false);
						player->learnSpell(11566, false);
						player->learnSpell(11567, false);
						player->learnSpell(11572, false);
						player->learnSpell(11573, false);
						player->learnSpell(11574, false);
						player->learnSpell(11578, false);
						player->learnSpell(11580, false);
						player->learnSpell(11581, false);
						player->learnSpell(11600, false);
						player->learnSpell(11601, false);
						player->learnSpell(11604, false);
						player->learnSpell(11605, false);
						player->learnSpell(11556, false);
						player->learnSpell(11555, false);
						player->learnSpell(11554, false);
						player->learnSpell(6552, false);
						player->learnSpell(6572, false);
						player->learnSpell(6574, false);
						player->learnSpell(6673, false);
						player->learnSpell(7369, false);
						player->learnSpell(7379, false);
						player->learnSpell(7384, false);
						player->learnSpell(8198, false);
						player->learnSpell(8204, false);
						player->learnSpell(8205, false);
						player->learnSpell(8820, false);
						player->learnSpell(11549, false);
						player->learnSpell(11550, false);
						player->learnSpell(11551, false);
						player->learnSpell(11608, false);
					}
					break;

					case CLASS_DEATH_KNIGHT: //Death Knight Spells Trainer 28474
					{
						player->learnSpell(48778, false);
						player->learnSpell(49998, false);
						player->learnSpell(50977, false);
						player->learnSpell(33388, false);
						player->learnSpell(33391, false);
						player->learnSpell(34090, false);
						player->learnSpell(34091, false);
						player->learnSpell(54197, false);
						player->learnSpell(200, false);
						player->learnSpell(202, false);
						player->learnSpell(196, false);
						player->learnSpell(197, false);
						player->learnSpell(198, false);
						player->learnSpell(199, false);
						player->learnSpell(51423, false);
						player->learnSpell(51424, false);
						player->learnSpell(51425, false);
						player->learnSpell(53323, false);
						player->learnSpell(49999, false);
						player->learnSpell(50842, false);
						player->learnSpell(53331, false);
						player->learnSpell(53341, false);
						player->learnSpell(56222, false);
						player->learnSpell(56815, false);
						player->learnSpell(57330, false);
						player->learnSpell(57623, false);
						player->learnSpell(61999, false);
						player->learnSpell(62158, false);
						player->learnSpell(53342, false);
						player->learnSpell(53343, false);
						player->learnSpell(53344, false);
						player->learnSpell(53428, false);
						player->learnSpell(54446, false);
						player->learnSpell(54447, false);
						player->learnSpell(70164, false);
						player->learnSpell(49941, false);
						player->learnSpell(3714, false);
						player->learnSpell(48721, false);
						player->learnSpell(48743, false);
						player->learnSpell(48792, false);
						player->learnSpell(49020, false);
						player->learnSpell(49892, false);
						player->learnSpell(49893, false);
						player->learnSpell(49894, false);
						player->learnSpell(49895, false);
						player->learnSpell(49896, false);
						player->learnSpell(48707, false);
						player->learnSpell(48265, false);
						player->learnSpell(48263, false);
						player->learnSpell(42650, false);
						player->learnSpell(43265, false);
						player->learnSpell(45463, false);
						player->learnSpell(45524, false);
						player->learnSpell(45529, false);
						player->learnSpell(46584, false);
						player->learnSpell(47476, false);
						player->learnSpell(47528, false);
						player->learnSpell(47568, false);
						player->learnSpell(49903, false);
						player->learnSpell(49904, false);
						player->learnSpell(49926, false);
						player->learnSpell(49927, false);
						player->learnSpell(49928, false);
						player->learnSpell(49929, false);
						player->learnSpell(49930, false);
						player->learnSpell(49936, false);
						player->learnSpell(49937, false);
						player->learnSpell(49938, false);
						player->learnSpell(49939, false);
						player->learnSpell(49924, false);
						player->learnSpell(49923, false);
						player->learnSpell(49921, false);
						player->learnSpell(49909, false);
						player->learnSpell(49917, false);
						player->learnSpell(49918, false);
						player->learnSpell(49919, false);
						player->learnSpell(49920, false);
						player->learnSpell(49940, false);
					}
					break;

					case CLASS_DRUID:	// Druid Spells Trainer 16655
					{
						player->learnSpell(27002, false);
						player->learnSpell(15590, false);
						player->learnSpell(200, false);
						player->learnSpell(62600, false);
						player->learnSpell(33388, false);
						player->learnSpell(33391, false);
						player->learnSpell(34090, false);
						player->learnSpell(34091, false);
						player->learnSpell(54197, false);
						player->learnSpell(227, false);
						player->learnSpell(1180, false);
						player->learnSpell(199, false);
						player->learnSpell(198, false);
						player->learnSpell(27001, false);
						player->learnSpell(27000, false);
						player->learnSpell(26998, false);
						player->learnSpell(26997, false);
						player->learnSpell(26996, false);
						player->learnSpell(26995, false);
						player->learnSpell(26994, false);
						player->learnSpell(26992, false);
						player->learnSpell(26991, false);
						player->learnSpell(26990, false);
						player->learnSpell(26989, false);
						player->learnSpell(26988, false);
						player->learnSpell(26987, false);
						player->learnSpell(26986, false);
						player->learnSpell(26985, false);
						player->learnSpell(27003, false);
						player->learnSpell(27004, false);
						player->learnSpell(27005, false);
						player->learnSpell(33950, false);
						player->learnSpell(33786, false);
						player->learnSpell(33763, false);
						player->learnSpell(33745, false);
						player->learnSpell(33357, false);
						player->learnSpell(31709, false);
						player->learnSpell(31018, false);
						player->learnSpell(29166, false);
						player->learnSpell(27012, false);
						player->learnSpell(27009, false);
						player->learnSpell(27008, false);
						player->learnSpell(27006, false);
						player->learnSpell(26984, false);
						player->learnSpell(26983, false);
						player->learnSpell(26982, false);
						player->learnSpell(21850, false);
						player->learnSpell(21849, false);
						player->learnSpell(20748, false);
						player->learnSpell(20747, false);
						player->learnSpell(20742, false);
						player->learnSpell(20739, false);
						player->learnSpell(20719, false);
						player->learnSpell(20484, false);
						player->learnSpell(18658, false);
						player->learnSpell(18657, false);
						player->learnSpell(17402, false);
						player->learnSpell(17401, false);
						player->learnSpell(17329, false);
						player->learnSpell(16914, false);
						player->learnSpell(16857, false);
						player->learnSpell(16813, false);
						player->learnSpell(22568, false);
						player->learnSpell(22570, false);
						player->learnSpell(22812, false);
						player->learnSpell(26981, false);
						player->learnSpell(26980, false);
						player->learnSpell(26979, false);
						player->learnSpell(26978, false);
						player->learnSpell(25299, false);
						player->learnSpell(25298, false);
						player->learnSpell(25297, false);
						player->learnSpell(24248, false);
						player->learnSpell(22842, false);
						player->learnSpell(22829, false);
						player->learnSpell(22828, false);
						player->learnSpell(22827, false);
						player->learnSpell(16812, false);
						player->learnSpell(50765, false);
						player->learnSpell(50764, false);
						player->learnSpell(50763, false);
						player->learnSpell(50464, false);
						player->learnSpell(50213, false);
						player->learnSpell(50212, false);
						player->learnSpell(49803, false);
						player->learnSpell(49802, false);
						player->learnSpell(49800, false);
						player->learnSpell(49799, false);
						player->learnSpell(48579, false);
						player->learnSpell(48578, false);
						player->learnSpell(48577, false);
						player->learnSpell(48576, false);
						player->learnSpell(48575, false);
						player->learnSpell(48574, false);
						player->learnSpell(50766, false);
						player->learnSpell(50767, false);
						player->learnSpell(50768, false);
						player->learnSpell(62078, false);
						player->learnSpell(53312, false);
						player->learnSpell(53308, false);
						player->learnSpell(53307, false);
						player->learnSpell(52610, false);
						player->learnSpell(50769, false);
						player->learnSpell(48573, false);
						player->learnSpell(48572, false);
						player->learnSpell(48571, false);
						player->learnSpell(48465, false);
						player->learnSpell(48464, false);
						player->learnSpell(48463, false);
						player->learnSpell(48462, false);
						player->learnSpell(48461, false);
						player->learnSpell(48459, false);
						player->learnSpell(48451, false);
						player->learnSpell(48450, false);
						player->learnSpell(48447, false);
						player->learnSpell(48446, false);
						player->learnSpell(48443, false);
						player->learnSpell(48442, false);
						player->learnSpell(48441, false);
						player->learnSpell(48440, false);
						player->learnSpell(48378, false);
						player->learnSpell(48377, false);
						player->learnSpell(48467, false);
						player->learnSpell(48469, false);
						player->learnSpell(48570, false);
						player->learnSpell(48569, false);
						player->learnSpell(48568, false);
						player->learnSpell(48567, false);
						player->learnSpell(48562, false);
						player->learnSpell(48561, false);
						player->learnSpell(48560, false);
						player->learnSpell(48559, false);
						player->learnSpell(48480, false);
						player->learnSpell(48479, false);
						player->learnSpell(48477, false);
						player->learnSpell(48470, false);
						player->learnSpell(40123, false);
						player->learnSpell(8924, false);
						player->learnSpell(5487, false);
						player->learnSpell(5234, false);
						player->learnSpell(5232, false);
						player->learnSpell(5229, false);
						player->learnSpell(5225, false);
						player->learnSpell(5221, false);
						player->learnSpell(5217, false);
						player->learnSpell(5215, false);
						player->learnSpell(5211, false);
						player->learnSpell(5209, false);
						player->learnSpell(5201, false);
						player->learnSpell(5196, false);
						player->learnSpell(5195, false);
						player->learnSpell(5189, false);
						player->learnSpell(5188, false);
						player->learnSpell(5187, false);
						player->learnSpell(6756, false);
						player->learnSpell(6778, false);
						player->learnSpell(6780, false);
						player->learnSpell(8921, false);
						player->learnSpell(8918, false);
						player->learnSpell(8914, false);
						player->learnSpell(8910, false);
						player->learnSpell(8907, false);
						player->learnSpell(8905, false);
						player->learnSpell(8903, false);
						player->learnSpell(6809, false);
						player->learnSpell(6808, false);
						player->learnSpell(6807, false);
						player->learnSpell(6800, false);
						player->learnSpell(6798, false);
						player->learnSpell(6795, false);
						player->learnSpell(6793, false);
						player->learnSpell(6787, false);
						player->learnSpell(6785, false);
						player->learnSpell(5186, false);
						player->learnSpell(5180, false);
						player->learnSpell(5179, false);
						player->learnSpell(1079, false);
						player->learnSpell(1075, false);
						player->learnSpell(1066, false);
						player->learnSpell(1062, false);
						player->learnSpell(1058, false);
						player->learnSpell(783, false);
						player->learnSpell(782, false);
						player->learnSpell(780, false);
						player->learnSpell(779, false);
						player->learnSpell(774, false);
						player->learnSpell(770, false);
						player->learnSpell(769, false);
						player->learnSpell(768, false);
						player->learnSpell(740, false);
						player->learnSpell(467, false);
						player->learnSpell(339, false);
						player->learnSpell(1082, false);
						player->learnSpell(1126, false);
						player->learnSpell(1430, false);
						player->learnSpell(5178, false);
						player->learnSpell(5177, false);
						player->learnSpell(3627, false);
						player->learnSpell(3029, false);
						player->learnSpell(2912, false);
						player->learnSpell(2908, false);
						player->learnSpell(2893, false);
						player->learnSpell(2782, false);
						player->learnSpell(2637, false);
						player->learnSpell(2091, false);
						player->learnSpell(2090, false);
						player->learnSpell(1850, false);
						player->learnSpell(1824, false);
						player->learnSpell(1823, false);
						player->learnSpell(1822, false);
						player->learnSpell(1735, false);
						player->learnSpell(99, false);
						player->learnSpell(16811, false);
						player->learnSpell(9867, false);
						player->learnSpell(9866, false);
						player->learnSpell(9863, false);
						player->learnSpell(9862, false);
						player->learnSpell(9858, false);
						player->learnSpell(9857, false);
						player->learnSpell(9856, false);
						player->learnSpell(9853, false);
						player->learnSpell(9852, false);
						player->learnSpell(9850, false);
						player->learnSpell(9849, false);
						player->learnSpell(9846, false);
						player->learnSpell(9845, false);
						player->learnSpell(9841, false);
						player->learnSpell(9840, false);
						player->learnSpell(9839, false);
						player->learnSpell(9875, false);
						player->learnSpell(9876, false);
						player->learnSpell(9880, false);
						player->learnSpell(16810, false);
						player->learnSpell(16689, false);
						player->learnSpell(9912, false);
						player->learnSpell(9910, false);
						player->learnSpell(9908, false);
						player->learnSpell(9904, false);
						player->learnSpell(9901, false);
						player->learnSpell(9898, false);
						player->learnSpell(9896, false);
						player->learnSpell(9894, false);
						player->learnSpell(9892, false);
						player->learnSpell(9889, false);
						player->learnSpell(9888, false);
						player->learnSpell(9885, false);
						player->learnSpell(9884, false);
						player->learnSpell(9881, false);
						player->learnSpell(9835, false);
						player->learnSpell(9834, false);
						player->learnSpell(9833, false);
						player->learnSpell(8992, false);
						player->learnSpell(8983, false);
						player->learnSpell(8972, false);
						player->learnSpell(8955, false);
						player->learnSpell(8951, false);
						player->learnSpell(8950, false);
						player->learnSpell(8949, false);
						player->learnSpell(8941, false);
						player->learnSpell(8940, false);
						player->learnSpell(8939, false);
						player->learnSpell(8938, false);
						player->learnSpell(8936, false);
						player->learnSpell(8929, false);
						player->learnSpell(8928, false);
						player->learnSpell(8927, false);
						player->learnSpell(8926, false);
						player->learnSpell(8998, false);
						player->learnSpell(9000, false);
						player->learnSpell(9005, false);
						player->learnSpell(9830, false);
						player->learnSpell(9829, false);
						player->learnSpell(9827, false);
						player->learnSpell(9823, false);
						player->learnSpell(9821, false);
						player->learnSpell(9758, false);
						player->learnSpell(9756, false);
						player->learnSpell(9754, false);
						player->learnSpell(9752, false);
						player->learnSpell(9750, false);
						player->learnSpell(9747, false);
						player->learnSpell(9745, false);
						player->learnSpell(9634, false);
						player->learnSpell(9493, false);
						player->learnSpell(9492, false);
						player->learnSpell(9490, false);
						player->learnSpell(8925, false);
					}
					break;

					case CLASS_HUNTER:		//Hunter Spells Trainer 5115
					{
						player->learnSpell(36916, false);
						player->learnSpell(674, false);
						player->learnSpell(2567, false);
						player->learnSpell(33388, false);
						player->learnSpell(33391, false);
						player->learnSpell(34090, false);
						player->learnSpell(34091, false);
						player->learnSpell(54197, false);
						player->learnSpell(5011, false);
						player->learnSpell(264, false);
						player->learnSpell(200, false);
						player->learnSpell(15590, false);
						player->learnSpell(266, false);
						player->learnSpell(227, false);
						player->learnSpell(1180, false);
						player->learnSpell(202, false);
						player->learnSpell(201, false);
						player->learnSpell(197, false);
						player->learnSpell(196, false);
						player->learnSpell(27025, false);
						player->learnSpell(27023, false);
						player->learnSpell(27022, false);
						player->learnSpell(27021, false);
						player->learnSpell(27019, false);
						player->learnSpell(27016, false);
						player->learnSpell(27014, false);
						player->learnSpell(25296, false);
						player->learnSpell(25295, false);
						player->learnSpell(27044, false);
						player->learnSpell(27045, false);
						player->learnSpell(34600, false);
						player->learnSpell(34477, false);
						player->learnSpell(34120, false);
						player->learnSpell(34074, false);
						player->learnSpell(34026, false);
						player->learnSpell(27046, false);
						player->learnSpell(25294, false);
						player->learnSpell(19885, false);
						player->learnSpell(19884, false);
						player->learnSpell(19883, false);
						player->learnSpell(19882, false);
						player->learnSpell(19880, false);
						player->learnSpell(19879, false);
						player->learnSpell(19878, false);
						player->learnSpell(19801, false);
						player->learnSpell(19263, false);
						player->learnSpell(20043, false);
						player->learnSpell(20190, false);
						player->learnSpell(20736, false);
						player->learnSpell(14327, false);
						player->learnSpell(58434, false);
						player->learnSpell(58431, false);
						player->learnSpell(56641, false);
						player->learnSpell(53351, false);
						player->learnSpell(53339, false);
						player->learnSpell(53338, false);
						player->learnSpell(53271, false);
						player->learnSpell(60192, false);
						player->learnSpell(62757, false);
						player->learnSpell(61847, false);
						player->learnSpell(61846, false);
						player->learnSpell(61006, false);
						player->learnSpell(61005, false);
						player->learnSpell(49071, false);
						player->learnSpell(49067, false);
						player->learnSpell(49001, false);
						player->learnSpell(49000, false);
						player->learnSpell(48996, false);
						player->learnSpell(48995, false);
						player->learnSpell(48990, false);
						player->learnSpell(49044, false);
						player->learnSpell(49045, false);
						player->learnSpell(49066, false);
						player->learnSpell(49056, false);
						player->learnSpell(49055, false);
						player->learnSpell(49052, false);
						player->learnSpell(49051, false);
						player->learnSpell(49048, false);
						player->learnSpell(49047, false);
						player->learnSpell(48989, false);
						player->learnSpell(14326, false);
						player->learnSpell(13552, false);
						player->learnSpell(6991, false);
						player->learnSpell(6197, false);
						player->learnSpell(5384, false);
						player->learnSpell(5118, false);
						player->learnSpell(5116, false);
						player->learnSpell(3662, false);
						player->learnSpell(3661, false);
						player->learnSpell(3127, false);
						player->learnSpell(3111, false);
						player->learnSpell(8737, false);
						player->learnSpell(13159, false);
						player->learnSpell(13551, false);
						player->learnSpell(13550, false);
						player->learnSpell(13549, false);
						player->learnSpell(13544, false);
						player->learnSpell(13543, false);
						player->learnSpell(13542, false);
						player->learnSpell(13165, false);
						player->learnSpell(13163, false);
						player->learnSpell(13161, false);
						player->learnSpell(3045, false);
						player->learnSpell(3044, false);
						player->learnSpell(1495, false);
						player->learnSpell(1494, false);
						player->learnSpell(1462, false);
						player->learnSpell(1130, false);
						player->learnSpell(1002, false);
						player->learnSpell(982, false);
						player->learnSpell(883, false);
						player->learnSpell(781, false);
						player->learnSpell(674, false);
						player->learnSpell(1499, false);
						player->learnSpell(1510, false);
						player->learnSpell(3043, false);
						player->learnSpell(3034, false);
						player->learnSpell(2974, false);
						player->learnSpell(2643, false);
						player->learnSpell(2641, false);
						player->learnSpell(1978, false);
						player->learnSpell(1543, false);
						player->learnSpell(1515, false);
						player->learnSpell(1513, false);
						player->learnSpell(136, false);
						player->learnSpell(14325, false);
						player->learnSpell(14305, false);
						player->learnSpell(14304, false);
						player->learnSpell(14303, false);
						player->learnSpell(14302, false);
						player->learnSpell(14295, false);
						player->learnSpell(14294, false);
						player->learnSpell(14290, false);
						player->learnSpell(14289, false);
						player->learnSpell(14288, false);
						player->learnSpell(14310, false);
						player->learnSpell(14311, false);
						player->learnSpell(14324, false);
						player->learnSpell(14323, false);
						player->learnSpell(14322, false);
						player->learnSpell(14321, false);
						player->learnSpell(14320, false);
						player->learnSpell(14319, false);
						player->learnSpell(14318, false);
						player->learnSpell(14317, false);
						player->learnSpell(14316, false);
						player->learnSpell(14287, false);
						player->learnSpell(14286, false);
						player->learnSpell(14263, false);
						player->learnSpell(14262, false);
						player->learnSpell(14261, false);
						player->learnSpell(14260, false);
						player->learnSpell(13813, false);
						player->learnSpell(13809, false);
						player->learnSpell(13795, false);
						player->learnSpell(13555, false);
						player->learnSpell(13554, false);
						player->learnSpell(14264, false);
						player->learnSpell(14265, false);
						player->learnSpell(14285, false);
						player->learnSpell(14284, false);
						player->learnSpell(14283, false);
						player->learnSpell(14282, false);
						player->learnSpell(14281, false);
						player->learnSpell(14271, false);
						player->learnSpell(14270, false);
						player->learnSpell(14269, false);
						player->learnSpell(14266, false);
						player->learnSpell(13553, false);
					}
					break;

					case CLASS_MAGE:	// Mage Spells Trainer 28958
					{
						player->learnSpell(33944, false);
						player->learnSpell(227, false);
						player->learnSpell(1180, false);
						player->learnSpell(33388, false);
						player->learnSpell(33391, false);
						player->learnSpell(34090, false);
						player->learnSpell(34091, false);
						player->learnSpell(54197, false);
						player->learnSpell(32271, false);
						player->learnSpell(3562, false);
						player->learnSpell(3561, false);
						player->learnSpell(28272, false);
						player->learnSpell(28271, false);
						player->learnSpell(3565, false);
						player->learnSpell(49360, false);
						player->learnSpell(49359, false);
						player->learnSpell(32266, false);
						player->learnSpell(11416, false);
						player->learnSpell(10059, false);
						player->learnSpell(11419, false);
						player->learnSpell(33690, false);
						player->learnSpell(33691, false);
						player->learnSpell(53140, false);
						player->learnSpell(53142, false);
						player->learnSpell(201, false);
						player->learnSpell(27131, false);
						player->learnSpell(27130, false);
						player->learnSpell(45438, false);
						player->learnSpell(61305, false);
						player->learnSpell(61721, false);
						player->learnSpell(27128, false);
						player->learnSpell(27127, false);
						player->learnSpell(27126, false);
						player->learnSpell(27125, false);
						player->learnSpell(27124, false);
						player->learnSpell(27101, false);
						player->learnSpell(27090, false);
						player->learnSpell(27088, false);
						player->learnSpell(27087, false);
						player->learnSpell(27086, false);
						player->learnSpell(27085, false);
						player->learnSpell(33717, false);
						player->learnSpell(32796, false);
						player->learnSpell(30482, false);
						player->learnSpell(30455, false);
						player->learnSpell(30451, false);
						player->learnSpell(30449, false);
						player->learnSpell(28612, false);
						player->learnSpell(28609, false);
						player->learnSpell(27082, false);
						player->learnSpell(27080, false);
						player->learnSpell(12826, false);
						player->learnSpell(12825, false);
						player->learnSpell(12824, false);
						player->learnSpell(27079, false);
						player->learnSpell(27078, false);
						player->learnSpell(27075, false);
						player->learnSpell(27074, false);
						player->learnSpell(27073, false);
						player->learnSpell(27072, false);
						player->learnSpell(27071, false);
						player->learnSpell(27070, false);
						player->learnSpell(25345, false);
						player->learnSpell(25306, false);
						player->learnSpell(25304, false);
						player->learnSpell(23028, false);
						player->learnSpell(22783, false);
						player->learnSpell(22782, false);
						player->learnSpell(58659, false);
						player->learnSpell(43019, false);
						player->learnSpell(43017, false);
						player->learnSpell(43015, false);
						player->learnSpell(43012, false);
						player->learnSpell(43010, false);
						player->learnSpell(43008, false);
						player->learnSpell(43002, false);
						player->learnSpell(42995, false);
						player->learnSpell(42985, false);
						player->learnSpell(42956, false);
						player->learnSpell(42955, false);
						player->learnSpell(43020, false);
						player->learnSpell(43023, false);
						player->learnSpell(55342, false);
						player->learnSpell(47610, false);
						player->learnSpell(44614, false);
						player->learnSpell(43987, false);
						player->learnSpell(43046, false);
						player->learnSpell(43045, false);
						player->learnSpell(43024, false);
						player->learnSpell(42940, false);
						player->learnSpell(42872, false);
						player->learnSpell(42859, false);
						player->learnSpell(42858, false);
						player->learnSpell(42846, false);
						player->learnSpell(42843, false);
						player->learnSpell(42842, false);
						player->learnSpell(42841, false);
						player->learnSpell(42833, false);
						player->learnSpell(42832, false);
						player->learnSpell(38704, false);
						player->learnSpell(38699, false);
						player->learnSpell(38697, false);
						player->learnSpell(38692, false);
						player->learnSpell(37420, false);
						player->learnSpell(42873, false);
						player->learnSpell(42939, false);
						player->learnSpell(42931, false);
						player->learnSpell(42930, false);
						player->learnSpell(42926, false);
						player->learnSpell(42925, false);
						player->learnSpell(42921, false);
						player->learnSpell(42920, false);
						player->learnSpell(42917, false);
						player->learnSpell(42914, false);
						player->learnSpell(42913, false);
						player->learnSpell(42897, false);
						player->learnSpell(42896, false);
						player->learnSpell(42894, false);
						player->learnSpell(33946, false);
						player->learnSpell(8423, false);
						player->learnSpell(6143, false);
						player->learnSpell(6141, false);
						player->learnSpell(6131, false);
						player->learnSpell(6129, false);
						player->learnSpell(6127, false);
						player->learnSpell(6117, false);
						player->learnSpell(5506, false);
						player->learnSpell(5505, false);
						player->learnSpell(5504, false);
						player->learnSpell(5145, false);
						player->learnSpell(5144, false);
						player->learnSpell(5143, false);
						player->learnSpell(3552, false);
						player->learnSpell(3140, false);
						player->learnSpell(7300, false);
						player->learnSpell(7301, false);
						player->learnSpell(8422, false);
						player->learnSpell(8417, false);
						player->learnSpell(8416, false);
						player->learnSpell(8413, false);
						player->learnSpell(8412, false);
						player->learnSpell(8408, false);
						player->learnSpell(8407, false);
						player->learnSpell(8406, false);
						player->learnSpell(8402, false);
						player->learnSpell(8401, false);
						player->learnSpell(8400, false);
						player->learnSpell(7322, false);
						player->learnSpell(7320, false);
						player->learnSpell(7302, false);
						player->learnSpell(2948, false);
						player->learnSpell(2139, false);
						player->learnSpell(604, false);
						player->learnSpell(597, false);
						player->learnSpell(587, false);
						player->learnSpell(543, false);
						player->learnSpell(475, false);
						player->learnSpell(205, false);
						player->learnSpell(145, false);
						player->learnSpell(143, false);
						player->learnSpell(130, false);
						player->learnSpell(122, false);
						player->learnSpell(120, false);
						player->learnSpell(118, false);
						player->learnSpell(116, false);
						player->learnSpell(66, false);
						player->learnSpell(759, false);
						player->learnSpell(837, false);
						player->learnSpell(2138, false);
						player->learnSpell(2137, false);
						player->learnSpell(2136, false);
						player->learnSpell(2121, false);
						player->learnSpell(2120, false);
						player->learnSpell(1953, false);
						player->learnSpell(1463, false);
						player->learnSpell(1461, false);
						player->learnSpell(1460, false);
						player->learnSpell(1459, false);
						player->learnSpell(1449, false);
						player->learnSpell(1008, false);
						player->learnSpell(990, false);
						player->learnSpell(865, false);
						player->learnSpell(10, false);
						player->learnSpell(12051, false);
						player->learnSpell(10193, false);
						player->learnSpell(10192, false);
						player->learnSpell(10191, false);
						player->learnSpell(10187, false);
						player->learnSpell(10186, false);
						player->learnSpell(10185, false);
						player->learnSpell(10181, false);
						player->learnSpell(10180, false);
						player->learnSpell(10179, false);
						player->learnSpell(10177, false);
						player->learnSpell(10174, false);
						player->learnSpell(10173, false);
						player->learnSpell(10170, false);
						player->learnSpell(10169, false);
						player->learnSpell(10197, false);
						player->learnSpell(10199, false);
						player->learnSpell(10230, false);
						player->learnSpell(10225, false);
						player->learnSpell(10223, false);
						player->learnSpell(10220, false);
						player->learnSpell(10219, false);
						player->learnSpell(10216, false);
						player->learnSpell(10215, false);
						player->learnSpell(10212, false);
						player->learnSpell(10211, false);
						player->learnSpell(10207, false);
						player->learnSpell(10206, false);
						player->learnSpell(10205, false);
						player->learnSpell(10202, false);
						player->learnSpell(10201, false);
						player->learnSpell(10161, false);
						player->learnSpell(10160, false);
						player->learnSpell(8492, false);
						player->learnSpell(8462, false);
						player->learnSpell(8461, false);
						player->learnSpell(8458, false);
						player->learnSpell(8457, false);
						player->learnSpell(8455, false);
						player->learnSpell(8451, false);
						player->learnSpell(8450, false);
						player->learnSpell(8446, false);
						player->learnSpell(8445, false);
						player->learnSpell(8444, false);
						player->learnSpell(8439, false);
						player->learnSpell(8438, false);
						player->learnSpell(8437, false);
						player->learnSpell(8494, false);
						player->learnSpell(8495, false);
						player->learnSpell(10159, false);
						player->learnSpell(10157, false);
						player->learnSpell(10156, false);
						player->learnSpell(10151, false);
						player->learnSpell(10150, false);
						player->learnSpell(10149, false);
						player->learnSpell(10148, false);
						player->learnSpell(10145, false);
						player->learnSpell(10144, false);
						player->learnSpell(10140, false);
						player->learnSpell(10139, false);
						player->learnSpell(10138, false);
						player->learnSpell(10054, false);
						player->learnSpell(10053, false);
						player->learnSpell(8427, false);
					}
					break;

					case CLASS_PALADIN:	// Paladin Spells Trainer 23128
					{
						player->learnSpell(465, false);
						player->learnSpell(200, false);
						player->learnSpell(33388, false);
						player->learnSpell(33391, false);
						player->learnSpell(34090, false);
						player->learnSpell(34091, false);
						player->learnSpell(54197, false);
						player->learnSpell(202, false);
						player->learnSpell(201, false);
						player->learnSpell(197, false);
						player->learnSpell(196, false);
						player->learnSpell(199, false);
						player->learnSpell(198, false);
						player->learnSpell(498, false);
						player->learnSpell(633, false);
						player->learnSpell(639, false);
						player->learnSpell(642, false);
						player->learnSpell(643, false);
						player->learnSpell(647, false);
						player->learnSpell(750, false);
						player->learnSpell(853, false);
						player->learnSpell(879, false);
						player->learnSpell(1022, false);
						player->learnSpell(1026, false);
						player->learnSpell(1032, false);
						player->learnSpell(1038, false);
						player->learnSpell(1042, false);
						player->learnSpell(1044, false);
						player->learnSpell(1152, false);
						player->learnSpell(2800, false);
						player->learnSpell(2812, false);
						player->learnSpell(3127, false);
						player->learnSpell(3472, false);
						player->learnSpell(4987, false);
						player->learnSpell(5502, false);
						player->learnSpell(5588, false);
						player->learnSpell(5589, false);
						player->learnSpell(5599, false);
						player->learnSpell(5614, false);
						player->learnSpell(5615, false);
						player->learnSpell(6940, false);
						player->learnSpell(7294, false);
						player->learnSpell(10278, false);
						player->learnSpell(10290, false);
						player->learnSpell(10291, false);
						player->learnSpell(10292, false);
						player->learnSpell(10293, false);
						player->learnSpell(10298, false);
						player->learnSpell(10299, false);
						player->learnSpell(10300, false);
						player->learnSpell(10301, false);
						player->learnSpell(10308, false);
						player->learnSpell(10310, false);
						player->learnSpell(10312, false);
						player->learnSpell(10313, false);
						player->learnSpell(10314, false);
						player->learnSpell(10318, false);
						player->learnSpell(10322, false);
						player->learnSpell(10324, false);
						player->learnSpell(10326, false);
						player->learnSpell(10328, false);
						player->learnSpell(10329, false);
						//player->learnSpell(13820, false);
						player->learnSpell(19740, false);
						player->learnSpell(19742, false);
						player->learnSpell(19746, false);
						player->learnSpell(19750, false);
						player->learnSpell(19752, false);
						player->learnSpell(19834, false);
						player->learnSpell(19835, false);
						player->learnSpell(19836, false);
						player->learnSpell(19837, false);
						player->learnSpell(19838, false);
						player->learnSpell(19850, false);
						player->learnSpell(19852, false);
						player->learnSpell(19853, false);
						player->learnSpell(19854, false);
						player->learnSpell(19876, false);
						player->learnSpell(19888, false);
						player->learnSpell(19891, false);
						player->learnSpell(19895, false);
						player->learnSpell(19896, false);
						player->learnSpell(19897, false);
						player->learnSpell(19898, false);
						player->learnSpell(19899, false);
						player->learnSpell(19900, false);
						player->learnSpell(19939, false);
						player->learnSpell(19940, false);
						player->learnSpell(19941, false);
						player->learnSpell(19942, false);
						player->learnSpell(19943, false);
						player->learnSpell(20116, false);
						player->learnSpell(20164, false);
						player->learnSpell(20165, false);
						player->learnSpell(20166, false);
						player->learnSpell(20217, false);
						player->learnSpell(20271, false);
						player->learnSpell(20772, false);
						player->learnSpell(20773, false);
						player->learnSpell(20922, false);
						player->learnSpell(20923, false);
						player->learnSpell(20924, false);
						//player->learnSpell(23215, false);
						player->learnSpell(24239, false);
						player->learnSpell(24274, false);
						player->learnSpell(24275, false);
						player->learnSpell(25290, false);
						player->learnSpell(25291, false);
						player->learnSpell(25292, false);
						player->learnSpell(25780, false);
						player->learnSpell(25782, false);
						player->learnSpell(25894, false);
						player->learnSpell(25898, false);
						player->learnSpell(25916, false);
						player->learnSpell(25918, false);
						player->learnSpell(26573, false);
						player->learnSpell(27135, false);
						player->learnSpell(27136, false);
						player->learnSpell(27137, false);
						player->learnSpell(27138, false);
						player->learnSpell(27139, false);
						player->learnSpell(27140, false);
						player->learnSpell(27141, false);
						player->learnSpell(27142, false);
						player->learnSpell(27143, false);
						player->learnSpell(27149, false);
						player->learnSpell(27150, false);
						player->learnSpell(27151, false);
						player->learnSpell(27152, false);
						player->learnSpell(27153, false);
						player->learnSpell(27154, false);
						player->learnSpell(27173, false);
						player->learnSpell(27180, false);
						player->learnSpell(31789, false);
						player->learnSpell(31801, false);
						player->learnSpell(31884, false);
						player->learnSpell(32223, false);
						if (player->GetTeam() == HORDE)
						{
							player->learnSpell(34767, false);
							player->learnSpell(34769, false);
						}
						else
						{
							player->learnSpell(23214, false);
							player->learnSpell(13819, false);
						}
						//player->learnSpell(34768, false);
						player->learnSpell(48781, false);
						player->learnSpell(48782, false);
						player->learnSpell(48784, false);
						player->learnSpell(48785, false);
						player->learnSpell(48788, false);
						player->learnSpell(48800, false);
						player->learnSpell(48801, false);
						player->learnSpell(48805, false);
						player->learnSpell(48806, false);
						player->learnSpell(48816, false);
						player->learnSpell(48817, false);
						player->learnSpell(48818, false);
						player->learnSpell(48819, false);
						player->learnSpell(48931, false);
						player->learnSpell(48932, false);
						player->learnSpell(48933, false);
						player->learnSpell(48934, false);
						player->learnSpell(48935, false);
						player->learnSpell(48936, false);
						player->learnSpell(48937, false);
						player->learnSpell(48938, false);
						player->learnSpell(48941, false);
						player->learnSpell(48942, false);
						player->learnSpell(48943, false);
						player->learnSpell(48945, false);
						player->learnSpell(48947, false);
						player->learnSpell(48949, false);
						player->learnSpell(48950, false);
						player->learnSpell(53407, false);
						player->learnSpell(53408, false);
						player->learnSpell(53600, false);
						player->learnSpell(53601, false);
						player->learnSpell(53736, false);
						player->learnSpell(54043, false);
						player->learnSpell(54428, false);
						player->learnSpell(61411, false);
						player->learnSpell(62124, false);
					}
					break;

					case CLASS_PRIEST: // Priest Spells Trainer 4090
					{
						player->learnSpell(17, false);
						player->learnSpell(227, false);
						player->learnSpell(1180, false);
						player->learnSpell(198, false);
						player->learnSpell(139, false);
						player->learnSpell(33388, false);
						player->learnSpell(33391, false);
						player->learnSpell(34090, false);
						player->learnSpell(34091, false);
						player->learnSpell(54197, false);
						player->learnSpell(453, false);
						player->learnSpell(527, false);
						player->learnSpell(528, false);
						player->learnSpell(552, false);
						player->learnSpell(586, false);
						player->learnSpell(588, false);
						player->learnSpell(589, false);
						player->learnSpell(591, false);
						player->learnSpell(592, false);
						player->learnSpell(594, false);
						player->learnSpell(596, false);
						player->learnSpell(598, false);
						player->learnSpell(600, false);
						player->learnSpell(602, false);
						player->learnSpell(605, false);
						player->learnSpell(970, false);
						player->learnSpell(976, false);
						player->learnSpell(984, false);
						player->learnSpell(988, false);
						player->learnSpell(992, false);
						player->learnSpell(996, false);
						player->learnSpell(1004, false);
						player->learnSpell(1006, false);
						player->learnSpell(1243, false);
						player->learnSpell(1244, false);
						player->learnSpell(1245, false);
						player->learnSpell(1706, false);
						player->learnSpell(2006, false);
						player->learnSpell(2010, false);
						player->learnSpell(2052, false);
						player->learnSpell(2053, false);
						player->learnSpell(2054, false);
						player->learnSpell(2055, false);
						player->learnSpell(2060, false);
						player->learnSpell(2061, false);
						player->learnSpell(2096, false);
						player->learnSpell(2767, false);
						player->learnSpell(2791, false);
						player->learnSpell(2944, false);
						player->learnSpell(3747, false);
						player->learnSpell(6060, false);
						player->learnSpell(6063, false);
						player->learnSpell(6064, false);
						player->learnSpell(6065, false);
						player->learnSpell(6066, false);
						player->learnSpell(6074, false);
						player->learnSpell(6075, false);
						player->learnSpell(6076, false);
						player->learnSpell(6077, false);
						player->learnSpell(6078, false);
						player->learnSpell(6346, false);
						player->learnSpell(7128, false);
						player->learnSpell(8092, false);
						player->learnSpell(8102, false);
						player->learnSpell(8103, false);
						player->learnSpell(8104, false);
						player->learnSpell(8105, false);
						player->learnSpell(8106, false);
						player->learnSpell(8122, false);
						player->learnSpell(8124, false);
						player->learnSpell(8129, false);
						player->learnSpell(9472, false);
						player->learnSpell(9473, false);
						player->learnSpell(9474, false);
						player->learnSpell(9484, false);
						player->learnSpell(9485, false);
						player->learnSpell(10880, false);
						player->learnSpell(10881, false);
						player->learnSpell(10888, false);
						player->learnSpell(10890, false);
						player->learnSpell(10892, false);
						player->learnSpell(10893, false);
						player->learnSpell(10894, false);
						player->learnSpell(10898, false);
						player->learnSpell(10899, false);
						player->learnSpell(10900, false);
						player->learnSpell(10901, false);
						player->learnSpell(10909, false);
						player->learnSpell(10915, false);
						player->learnSpell(10916, false);
						player->learnSpell(10917, false);
						player->learnSpell(10927, false);
						player->learnSpell(10928, false);
						player->learnSpell(10929, false);
						player->learnSpell(10933, false);
						player->learnSpell(10934, false);
						player->learnSpell(10937, false);
						player->learnSpell(10938, false);
						player->learnSpell(10945, false);
						player->learnSpell(10946, false);
						player->learnSpell(10947, false);
						player->learnSpell(10951, false);
						player->learnSpell(10952, false);
						player->learnSpell(10955, false);
						player->learnSpell(10957, false);
						player->learnSpell(10958, false);
						player->learnSpell(10960, false);
						player->learnSpell(10961, false);
						player->learnSpell(10963, false);
						player->learnSpell(10964, false);
						player->learnSpell(10965, false);
						player->learnSpell(14752, false);
						player->learnSpell(14818, false);
						player->learnSpell(14819, false);
						player->learnSpell(14914, false);
						player->learnSpell(15237, false);
						player->learnSpell(15261, false);
						player->learnSpell(15262, false);
						player->learnSpell(15263, false);
						player->learnSpell(15264, false);
						player->learnSpell(15265, false);
						player->learnSpell(15266, false);
						player->learnSpell(15267, false);
						player->learnSpell(15430, false);
						player->learnSpell(15431, false);
						player->learnSpell(19276, false);
						player->learnSpell(19277, false);
						player->learnSpell(19278, false);
						player->learnSpell(19279, false);
						player->learnSpell(19280, false);
						player->learnSpell(20770, false);
						player->learnSpell(21562, false);
						player->learnSpell(21564, false);
						player->learnSpell(25210, false);
						player->learnSpell(25213, false);
						player->learnSpell(25217, false);
						player->learnSpell(25218, false);
						player->learnSpell(25221, false);
						player->learnSpell(25222, false);
						player->learnSpell(25233, false);
						player->learnSpell(25235, false);
						player->learnSpell(25308, false);
						player->learnSpell(25312, false);
						player->learnSpell(25314, false);
						player->learnSpell(25315, false);
						player->learnSpell(25316, false);
						player->learnSpell(25331, false);
						player->learnSpell(25363, false);
						player->learnSpell(25364, false);
						player->learnSpell(25367, false);
						player->learnSpell(25368, false);
						player->learnSpell(25372, false);
						player->learnSpell(25375, false);
						player->learnSpell(25384, false);
						player->learnSpell(25389, false);
						player->learnSpell(25392, false);
						player->learnSpell(25431, false);
						player->learnSpell(25433, false);
						player->learnSpell(25435, false);
						player->learnSpell(25467, false);
						player->learnSpell(27681, false);
						player->learnSpell(27683, false);
						player->learnSpell(27799, false);
						player->learnSpell(27800, false);
						player->learnSpell(27801, false);
						player->learnSpell(27841, false);
						player->learnSpell(32375, false);
						player->learnSpell(32379, false);
						player->learnSpell(32546, false);
						player->learnSpell(32996, false);
						player->learnSpell(32999, false);
						player->learnSpell(33076, false);
						player->learnSpell(34433, false);
						player->learnSpell(39374, false);
						player->learnSpell(48040, false);
						player->learnSpell(48045, false);
						player->learnSpell(48062, false);
						player->learnSpell(48063, false);
						player->learnSpell(48065, false);
						player->learnSpell(48066, false);
						player->learnSpell(48067, false);
						player->learnSpell(48068, false);
						player->learnSpell(48070, false);
						player->learnSpell(48071, false);
						player->learnSpell(48072, false);
						player->learnSpell(48073, false);
						player->learnSpell(48074, false);
						player->learnSpell(48077, false);
						player->learnSpell(48078, false);
						player->learnSpell(48112, false);
						player->learnSpell(48113, false);
						player->learnSpell(48119, false);
						player->learnSpell(48120, false);
						player->learnSpell(48122, false);
						player->learnSpell(48123, false);
						player->learnSpell(48124, false);
						player->learnSpell(48125, false);
						player->learnSpell(48126, false);
						player->learnSpell(48127, false);
						player->learnSpell(48134, false);
						player->learnSpell(48135, false);
						player->learnSpell(48157, false);
						player->learnSpell(48158, false);
						player->learnSpell(48161, false);
						player->learnSpell(48162, false);
						player->learnSpell(48168, false);
						player->learnSpell(48169, false);
						player->learnSpell(48170, false);
						player->learnSpell(48171, false);
						player->learnSpell(48299, false);
						player->learnSpell(48300, false);
						player->learnSpell(53023, false);
						player->learnSpell(64843, false);
						player->learnSpell(64901, false);
					}
					break;

					case CLASS_ROGUE:	// Rogue Spells Trainer 4584
					{
						player->learnSpell(53, false);
						player->learnSpell(674, false);
						player->learnSpell(2567, false);
						player->learnSpell(33388, false);
						player->learnSpell(33391, false);
						player->learnSpell(34090, false);
						player->learnSpell(34091, false);
						player->learnSpell(54197, false);
						player->learnSpell(5011, false);
						player->learnSpell(264, false);
						player->learnSpell(15590, false);
						player->learnSpell(266, false);
						player->learnSpell(1180, false);
						player->learnSpell(201, false);
						player->learnSpell(196, false);
						player->learnSpell(198, false);
						player->learnSpell(3018, false);
						player->learnSpell(408, false);
						player->learnSpell(674, false);
						player->learnSpell(703, false);
						player->learnSpell(921, false);
						player->learnSpell(1725, false);
						player->learnSpell(1757, false);
						player->learnSpell(1758, false);
						player->learnSpell(1759, false);
						player->learnSpell(1760, false);
						player->learnSpell(1766, false);
						player->learnSpell(1776, false);
						player->learnSpell(1784, false);
						player->learnSpell(1804, false);
						player->learnSpell(1833, false);
						player->learnSpell(1842, false);
						player->learnSpell(1856, false);
						player->learnSpell(1857, false);
						player->learnSpell(1860, false);
						player->learnSpell(1943, false);
						player->learnSpell(1966, false);
						player->learnSpell(2070, false);
						player->learnSpell(2094, false);
						player->learnSpell(2589, false);
						player->learnSpell(2590, false);
						player->learnSpell(2591, false);
						player->learnSpell(2836, false);
						player->learnSpell(2983, false);
						player->learnSpell(3127, false);
						player->learnSpell(5171, false);
						player->learnSpell(5277, false);
						player->learnSpell(5938, false);
						player->learnSpell(6760, false);
						player->learnSpell(6761, false);
						player->learnSpell(6762, false);
						player->learnSpell(6768, false);
						player->learnSpell(6770, false);
						player->learnSpell(6774, false);
						player->learnSpell(8621, false);
						player->learnSpell(8623, false);
						player->learnSpell(8624, false);
						player->learnSpell(8631, false);
						player->learnSpell(8632, false);
						player->learnSpell(8633, false);
						player->learnSpell(8637, false);
						player->learnSpell(8639, false);
						player->learnSpell(8640, false);
						player->learnSpell(8643, false);
						player->learnSpell(8647, false);
						player->learnSpell(8676, false);
						player->learnSpell(8696, false);
						player->learnSpell(8721, false);
						player->learnSpell(8724, false);
						player->learnSpell(8725, false);
						player->learnSpell(11267, false);
						player->learnSpell(11268, false);
						player->learnSpell(11269, false);
						player->learnSpell(11273, false);
						player->learnSpell(11274, false);
						player->learnSpell(11275, false);
						player->learnSpell(11279, false);
						player->learnSpell(11280, false);
						player->learnSpell(11281, false);
						player->learnSpell(11289, false);
						player->learnSpell(11290, false);
						player->learnSpell(11293, false);
						player->learnSpell(11294, false);
						player->learnSpell(11297, false);
						player->learnSpell(11299, false);
						player->learnSpell(11300, false);
						player->learnSpell(11303, false);
						player->learnSpell(11305, false);
						player->learnSpell(25300, false);
						player->learnSpell(25302, false);
						player->learnSpell(26669, false);
						player->learnSpell(26679, false);
						player->learnSpell(26839, false);
						player->learnSpell(26861, false);
						player->learnSpell(26862, false);
						player->learnSpell(26863, false);
						player->learnSpell(26865, false);
						player->learnSpell(26867, false);
						player->learnSpell(26884, false);
						player->learnSpell(26889, false);
						player->learnSpell(27441, false);
						player->learnSpell(27448, false);
						player->learnSpell(31016, false);
						player->learnSpell(31224, false);
						player->learnSpell(32645, false);
						player->learnSpell(32684, false);
						player->learnSpell(48637, false);
						player->learnSpell(48638, false);
						player->learnSpell(48656, false);
						player->learnSpell(48657, false);
						player->learnSpell(48658, false);
						player->learnSpell(48659, false);
						player->learnSpell(48667, false);
						player->learnSpell(48668, false);
						player->learnSpell(48671, false);
						player->learnSpell(48672, false);
						player->learnSpell(48673, false);
						player->learnSpell(48674, false);
						player->learnSpell(48675, false);
						player->learnSpell(48676, false);
						player->learnSpell(48689, false);
						player->learnSpell(48690, false);
						player->learnSpell(48691, false);
						player->learnSpell(51722, false);
						player->learnSpell(51723, false);
						player->learnSpell(51724, false);
						player->learnSpell(57934, false);
						player->learnSpell(57992, false);
						player->learnSpell(57993, false);
					}
					break;

					case CLASS_SHAMAN: // Shaman Spells Trainer 17204
					{
						player->learnSpell(131, false);
						player->learnSpell(15590, false);
						player->learnSpell(227, false);
						player->learnSpell(1180, false);
						player->learnSpell(197, false);
						player->learnSpell(196, false);
						player->learnSpell(199, false);
						player->learnSpell(33388, false);
						player->learnSpell(33391, false);
						player->learnSpell(34090, false);
						player->learnSpell(34091, false);
						player->learnSpell(54197, false);
						player->learnSpell(198, false);
						player->learnSpell(324, false);
						player->learnSpell(325, false);
						player->learnSpell(332, false);
						player->learnSpell(370, false);
						player->learnSpell(421, false);
						player->learnSpell(526, false);
						player->learnSpell(529, false);
						player->learnSpell(546, false);
						player->learnSpell(547, false);
						player->learnSpell(548, false);
						player->learnSpell(556, false);
						player->learnSpell(905, false);
						player->learnSpell(913, false);
						player->learnSpell(915, false);
						player->learnSpell(930, false);
						player->learnSpell(939, false);
						player->learnSpell(943, false);
						player->learnSpell(945, false);
						player->learnSpell(959, false);
						player->learnSpell(1064, false);
						player->learnSpell(1535, false);
						player->learnSpell(2008, false);
						player->learnSpell(2062, false);
						player->learnSpell(2484, false);
						player->learnSpell(2645, false);
						player->learnSpell(2860, false);
						player->learnSpell(2894, false);
						player->learnSpell(3738, false);
						player->learnSpell(5675, false);
						player->learnSpell(5730, false);
						player->learnSpell(6041, false);
						player->learnSpell(6196, false);
						player->learnSpell(6363, false);
						player->learnSpell(6364, false);
						player->learnSpell(6365, false);
						player->learnSpell(6375, false);
						player->learnSpell(6377, false);
						player->learnSpell(6390, false);
						player->learnSpell(6391, false);
						player->learnSpell(6392, false);
						player->learnSpell(6495, false);
						player->learnSpell(8004, false);
						player->learnSpell(8005, false);
						player->learnSpell(8008, false);
						player->learnSpell(8010, false);
						player->learnSpell(8012, false);
						player->learnSpell(8017, false);
						player->learnSpell(8018, false);
						player->learnSpell(8019, false);
						player->learnSpell(8024, false);
						player->learnSpell(8027, false);
						player->learnSpell(8030, false);
						player->learnSpell(8033, false);
						player->learnSpell(8038, false);
						player->learnSpell(8042, false);
						player->learnSpell(8044, false);
						player->learnSpell(8045, false);
						player->learnSpell(8046, false);
						player->learnSpell(8050, false);
						player->learnSpell(8052, false);
						player->learnSpell(8053, false);
						player->learnSpell(8056, false);
						player->learnSpell(8058, false);
						player->learnSpell(8075, false);
						player->learnSpell(8134, false);
						player->learnSpell(8143, false);
						player->learnSpell(8154, false);
						player->learnSpell(8155, false);
						player->learnSpell(8160, false);
						player->learnSpell(8161, false);
						player->learnSpell(8170, false);
						player->learnSpell(8177, false);
						player->learnSpell(8181, false);
						player->learnSpell(8184, false);
						player->learnSpell(8190, false);
						player->learnSpell(8227, false);
						player->learnSpell(8232, false);
						player->learnSpell(8235, false);
						player->learnSpell(8249, false);
						player->learnSpell(8498, false);
						player->learnSpell(8499, false);
						player->learnSpell(8512, false);
						player->learnSpell(8737, false);
						player->learnSpell(10391, false);
						player->learnSpell(10392, false);
						player->learnSpell(10395, false);
						player->learnSpell(10396, false);
						player->learnSpell(10399, false);
						player->learnSpell(10406, false);
						player->learnSpell(10407, false);
						player->learnSpell(10408, false);
						player->learnSpell(10412, false);
						player->learnSpell(10413, false);
						player->learnSpell(10414, false);
						player->learnSpell(10427, false);
						player->learnSpell(10428, false);
						player->learnSpell(10431, false);
						player->learnSpell(10432, false);
						player->learnSpell(10437, false);
						player->learnSpell(10438, false);
						player->learnSpell(10442, false);
						player->learnSpell(10447, false);
						player->learnSpell(10448, false);
						player->learnSpell(10456, false);
						player->learnSpell(10462, false);
						player->learnSpell(10463, false);
						player->learnSpell(10466, false);
						player->learnSpell(10467, false);
						player->learnSpell(10468, false);
						player->learnSpell(10472, false);
						player->learnSpell(10473, false);
						player->learnSpell(10478, false);
						player->learnSpell(10479, false);
						player->learnSpell(10486, false);
						player->learnSpell(10495, false);
						player->learnSpell(10496, false);
						player->learnSpell(10497, false);
						player->learnSpell(10526, false);
						player->learnSpell(10537, false);
						player->learnSpell(10538, false);
						player->learnSpell(10585, false);
						player->learnSpell(10586, false);
						player->learnSpell(10587, false);
						player->learnSpell(10595, false);
						player->learnSpell(10600, false);
						player->learnSpell(10601, false);
						player->learnSpell(10605, false);
						player->learnSpell(10622, false);
						player->learnSpell(10623, false);
						player->learnSpell(11314, false);
						player->learnSpell(11315, false);
						player->learnSpell(15207, false);
						player->learnSpell(15208, false);
						player->learnSpell(16339, false);
						player->learnSpell(16341, false);
						player->learnSpell(16342, false);
						player->learnSpell(16355, false);
						player->learnSpell(16356, false);
						player->learnSpell(16362, false);
						player->learnSpell(16387, false);
						player->learnSpell(20608, false);
						player->learnSpell(20609, false);
						player->learnSpell(20610, false);
						player->learnSpell(20776, false);
						player->learnSpell(20777, false);
						player->learnSpell(24398, false);
						player->learnSpell(25357, false);
						player->learnSpell(25361, false);
						player->learnSpell(25391, false);
						player->learnSpell(25396, false);
						player->learnSpell(25420, false);
						player->learnSpell(25422, false);
						player->learnSpell(25423, false);
						player->learnSpell(25439, false);
						player->learnSpell(25442, false);
						player->learnSpell(25448, false);
						player->learnSpell(25449, false);
						player->learnSpell(25454, false);
						player->learnSpell(25457, false);
						player->learnSpell(25464, false);
						player->learnSpell(25469, false);
						player->learnSpell(25472, false);
						player->learnSpell(25489, false);
						player->learnSpell(25500, false);
						player->learnSpell(25505, false);
						player->learnSpell(25508, false);
						player->learnSpell(25509, false);
						player->learnSpell(25525, false);
						player->learnSpell(25528, false);
						player->learnSpell(25533, false);
						player->learnSpell(25546, false);
						player->learnSpell(25547, false);
						player->learnSpell(25552, false);
						player->learnSpell(25557, false);
						player->learnSpell(25560, false);
						player->learnSpell(25563, false);
						player->learnSpell(25567, false);
						player->learnSpell(25570, false);
						player->learnSpell(25574, false);
						player->learnSpell(25590, false);
						player->learnSpell(29228, false);
						if (player->GetTeam() == ALLIANCE)
							player->learnSpell(32182, false);
						if (player->GetTeam() == HORDE)
							player->learnSpell(2825, false);
						player->learnSpell(33736, false);
						player->learnSpell(36936, false);
						player->learnSpell(49230, false);
						player->learnSpell(49231, false);
						player->learnSpell(49232, false);
						player->learnSpell(49233, false);
						player->learnSpell(49235, false);
						player->learnSpell(49236, false);
						player->learnSpell(49237, false);
						player->learnSpell(49238, false);
						player->learnSpell(49270, false);
						player->learnSpell(49271, false);
						player->learnSpell(49272, false);
						player->learnSpell(49273, false);
						player->learnSpell(49275, false);
						player->learnSpell(49276, false);
						player->learnSpell(49277, false);
						player->learnSpell(49280, false);
						player->learnSpell(49281, false);
						player->learnSpell(51505, false);
						player->learnSpell(51514, false);
						player->learnSpell(51730, false);
						player->learnSpell(51988, false);
						player->learnSpell(51991, false);
						player->learnSpell(51992, false);
						player->learnSpell(51993, false);
						player->learnSpell(51994, false);
						player->learnSpell(52127, false);
						player->learnSpell(52129, false);
						player->learnSpell(52131, false);
						player->learnSpell(52134, false);
						player->learnSpell(52136, false);
						player->learnSpell(52138, false);
						player->learnSpell(55458, false);
						player->learnSpell(55459, false);
						player->learnSpell(57622, false);
						player->learnSpell(57960, false);
						player->learnSpell(57994, false);
						player->learnSpell(58580, false);
						player->learnSpell(58581, false);
						player->learnSpell(58582, false);
						player->learnSpell(58643, false);
						player->learnSpell(58649, false);
						player->learnSpell(58652, false);
						player->learnSpell(58656, false);
						player->learnSpell(58699, false);
						player->learnSpell(58703, false);
						player->learnSpell(58704, false);
						player->learnSpell(58731, false);
						player->learnSpell(58734, false);
						player->learnSpell(58737, false);
						player->learnSpell(58739, false);
						player->learnSpell(58741, false);
						player->learnSpell(58745, false);
						player->learnSpell(58746, false);
						player->learnSpell(58749, false);
						player->learnSpell(58751, false);
						player->learnSpell(58753, false);
						player->learnSpell(58755, false);
						player->learnSpell(58756, false);
						player->learnSpell(58757, false);
						player->learnSpell(58771, false);
						player->learnSpell(58773, false);
						player->learnSpell(58774, false);
						player->learnSpell(58785, false);
						player->learnSpell(58789, false);
						player->learnSpell(58790, false);
						player->learnSpell(58794, false);
						player->learnSpell(58795, false);
						player->learnSpell(58796, false);
						player->learnSpell(58801, false);
						player->learnSpell(58803, false);
						player->learnSpell(58804, false);
						player->learnSpell(60043, false);
						player->learnSpell(61649, false);
						player->learnSpell(61657, false);
						player->learnSpell(66842, false);
						player->learnSpell(66843, false);
						player->learnSpell(66844, false);
					}
					break;

					case CLASS_WARLOCK: // Warlock Spells Trainer 23534
					{
						player->learnSpell(126, false);
						player->learnSpell(33388, false);
						player->learnSpell(33391, false);
						player->learnSpell(59671, false);
						player->learnSpell(50589, false);
						player->learnSpell(34090, false);
						player->learnSpell(54785, false);
						player->learnSpell(34091, false);
						player->learnSpell(47897, false);
						player->learnSpell(18540, false);
						player->learnSpell(1122, false);
						player->learnSpell(697, false);
						player->learnSpell(688, false);
						player->learnSpell(691, false);
						player->learnSpell(712, false);
						player->learnSpell(61290, false);
						player->learnSpell(54197, false);
						player->learnSpell(227, false);
						player->learnSpell(1180, false);
						player->learnSpell(201, false);
						player->learnSpell(132, false);
						player->learnSpell(172, false);
						player->learnSpell(348, false);
						player->learnSpell(603, false);
						player->learnSpell(688, false);
						player->learnSpell(689, false);
						player->learnSpell(693, false);
						player->learnSpell(695, false);
						player->learnSpell(696, false);
						player->learnSpell(698, false);
						player->learnSpell(699, false);
						player->learnSpell(702, false);
						player->learnSpell(705, false);
						player->learnSpell(706, false);
						player->learnSpell(707, false);
						player->learnSpell(709, false);
						player->learnSpell(710, false);
						player->learnSpell(755, false);
						player->learnSpell(980, false);
						player->learnSpell(1014, false);
						player->learnSpell(1086, false);
						player->learnSpell(1088, false);
						player->learnSpell(1094, false);
						player->learnSpell(1098, false);
						player->learnSpell(1106, false);
						player->learnSpell(1108, false);
						player->learnSpell(1120, false);
						player->learnSpell(1454, false);
						player->learnSpell(1455, false);
						player->learnSpell(1456, false);
						player->learnSpell(1490, false);
						player->learnSpell(1710, false);
						player->learnSpell(1714, false);
						player->learnSpell(1949, false);
						player->learnSpell(2362, false);
						player->learnSpell(2941, false);
						player->learnSpell(3698, false);
						player->learnSpell(3699, false);
						player->learnSpell(3700, false);
						player->learnSpell(5138, false);
						player->learnSpell(5484, false);
						player->learnSpell(5500, false);
						player->learnSpell(5676, false);
						player->learnSpell(5697, false);
						player->learnSpell(5699, false);
						player->learnSpell(5740, false);
						player->learnSpell(5782, false);
						player->learnSpell(6201, false);
						player->learnSpell(6202, false);
						player->learnSpell(6205, false);
						player->learnSpell(6213, false);
						player->learnSpell(6215, false);
						player->learnSpell(6217, false);
						player->learnSpell(6219, false);
						player->learnSpell(6222, false);
						player->learnSpell(6223, false);
						player->learnSpell(6229, false);
						player->learnSpell(6353, false);
						player->learnSpell(6366, false);
						player->learnSpell(6789, false);
						player->learnSpell(7641, false);
						player->learnSpell(7646, false);
						player->learnSpell(7648, false);
						player->learnSpell(7651, false);
						player->learnSpell(8288, false);
						player->learnSpell(8289, false);
						player->learnSpell(11659, false);
						player->learnSpell(11660, false);
						player->learnSpell(11661, false);
						player->learnSpell(11665, false);
						player->learnSpell(11667, false);
						player->learnSpell(11668, false);
						player->learnSpell(11671, false);
						player->learnSpell(11672, false);
						player->learnSpell(11675, false);
						player->learnSpell(11677, false);
						player->learnSpell(11678, false);
						player->learnSpell(11683, false);
						player->learnSpell(11684, false);
						player->learnSpell(11687, false);
						player->learnSpell(11688, false);
						player->learnSpell(11689, false);
						player->learnSpell(11693, false);
						player->learnSpell(11694, false);
						player->learnSpell(11695, false);
						player->learnSpell(11699, false);
						player->learnSpell(11700, false);
						player->learnSpell(11707, false);
						player->learnSpell(11708, false);
						player->learnSpell(11711, false);
						player->learnSpell(11712, false);
						player->learnSpell(11713, false);
						player->learnSpell(11719, false);
						player->learnSpell(11721, false);
						player->learnSpell(11722, false);
						player->learnSpell(11725, false);
						player->learnSpell(11726, false);
						player->learnSpell(11729, false);
						player->learnSpell(11730, false);
						player->learnSpell(11733, false);
						player->learnSpell(11734, false);
						player->learnSpell(11735, false);
						player->learnSpell(11739, false);
						player->learnSpell(11740, false);
						player->learnSpell(17727, false);
						player->learnSpell(17728, false);
						player->learnSpell(17919, false);
						player->learnSpell(17920, false);
						player->learnSpell(17921, false);
						player->learnSpell(17922, false);
						player->learnSpell(17923, false);
						player->learnSpell(17924, false);
						player->learnSpell(17925, false);
						player->learnSpell(17926, false);
						player->learnSpell(17928, false);
						player->learnSpell(17951, false);
						player->learnSpell(17952, false);
						player->learnSpell(17953, false);
						player->learnSpell(18647, false);
						player->learnSpell(20752, false);
						player->learnSpell(20755, false);
						player->learnSpell(20756, false);
						player->learnSpell(20757, false);
						player->learnSpell(23161, false);
						player->learnSpell(25307, false);
						player->learnSpell(25309, false);
						player->learnSpell(25311, false);
						player->learnSpell(27209, false);
						player->learnSpell(27210, false);
						player->learnSpell(27211, false);
						player->learnSpell(27212, false);
						player->learnSpell(27213, false);
						player->learnSpell(27215, false);
						player->learnSpell(27216, false);
						player->learnSpell(27217, false);
						player->learnSpell(27218, false);
						player->learnSpell(27219, false);
						player->learnSpell(27220, false);
						player->learnSpell(27222, false);
						player->learnSpell(27223, false);
						player->learnSpell(27224, false);
						player->learnSpell(27228, false);
						player->learnSpell(27230, false);
						player->learnSpell(27238, false);
						player->learnSpell(27243, false);
						player->learnSpell(27250, false);
						player->learnSpell(27259, false);
						player->learnSpell(27260, false);
						player->learnSpell(28172, false);
						player->learnSpell(28176, false);
						player->learnSpell(28189, false);
						player->learnSpell(28610, false);
						player->learnSpell(29722, false);
						player->learnSpell(29858, false);
						player->learnSpell(29893, false);
						player->learnSpell(30459, false);
						player->learnSpell(30545, false);
						player->learnSpell(30909, false);
						player->learnSpell(30910, false);
						player->learnSpell(32231, false);
						player->learnSpell(47793, false);
						player->learnSpell(47808, false);
						player->learnSpell(47809, false);
						player->learnSpell(47810, false);
						player->learnSpell(47811, false);
						player->learnSpell(47812, false);
						player->learnSpell(47813, false);
						player->learnSpell(47814, false);
						player->learnSpell(47815, false);
						player->learnSpell(47819, false);
						player->learnSpell(47820, false);
						player->learnSpell(47823, false);
						player->learnSpell(47824, false);
						player->learnSpell(47825, false);
						player->learnSpell(47835, false);
						player->learnSpell(47836, false);
						player->learnSpell(47837, false);
						player->learnSpell(47838, false);
						player->learnSpell(47855, false);
						player->learnSpell(47856, false);
						player->learnSpell(47857, false);
						player->learnSpell(47859, false);
						player->learnSpell(47860, false);
						player->learnSpell(47863, false);
						player->learnSpell(47864, false);
						player->learnSpell(47865, false);
						player->learnSpell(47867, false);
						player->learnSpell(47871, false);
						player->learnSpell(47878, false);
						player->learnSpell(47884, false);
						player->learnSpell(47886, false);
						player->learnSpell(47888, false);
						player->learnSpell(47889, false);
						player->learnSpell(47890, false);
						player->learnSpell(47891, false);
						player->learnSpell(47892, false);
						player->learnSpell(47893, false);
						player->learnSpell(48018, false);
						player->learnSpell(48020, false);
						player->learnSpell(50511, false);
						player->learnSpell(57946, false);
						player->learnSpell(58887, false);
						player->learnSpell(60219, false);
						player->learnSpell(60220, false);
						player->learnSpell(61191, false);
					}
					break;
				default:
					player->GetSession()->SendAreaTriggerMessage("Error: Contacte con un Administrador");
					return;
				break;
			}
		player->GetSession()->SendAreaTriggerMessage("Habilidades aprendidas");
		player->UpdateSkillsToMaxSkillsForLevel();
	}

	uint32 GetGuidByDb_item(const char* name)
	{
		QueryResult check = CharacterDatabase.PQuery("SELECT `guid` FROM `characters` WHERE `name` = '%s'", name);

		if(!check)
			return NULL;

		Field* fields = check->Fetch();
			return fields[0].GetUInt64();
	}

	uint32 GetTeamByDb_item(uint64 GUID, uint32 type)
	{
		QueryResult result = CharacterDatabase.PQuery("SELECT `arenaTeamId` FROM `arena_team_member` WHERE `guid` = %u", GUID);

		if(!result)
			return NULL;

		std::vector<ArenaTeam*> teamIds;

		do
		{
			Field* fields = result->Fetch();
			teamIds.push_back(sArenaTeamMgr->GetArenaTeamById(fields[0].GetUInt32()));
		}while(result->NextRow());

		for(RankingContainer::iterator itr = teamIds.begin(); itr != teamIds.end(); ++itr)
		{
			if((*itr)->GetType() == type)
				return (*itr)->GetId();
		}

		return NULL;
	}

	uint32 GetArenaTeamId_item(uint64 Guid, uint32 type)
	{
		Player* player = ObjectAccessor::FindPlayer(Guid);

		if(!player || !player->GetSession())
			return GetTeamByDb_item(Guid, type);

		return player->GetArenaTeamId(ArenaTeam::GetSlotByType(type));
	}

	std::string GetTypeAsString_item(uint32 type)
	{
		if(type == 2)
			return "2v2";
		else if(type == 3)
			return "3v3";
		return "5v5";
	}

	std::string getPlayerStatus_item(uint32 guid)
	{
		Player *player = sObjectAccessor->FindPlayer(guid);
		if(!player)
			return "Offline";
		if(player->isAFK())
			return "Online, <AFK> " + player->afkMsg;
		if(player->isDND())
		   return "Online, <Ocupado> " + player->dndMsg;
		return "Online";
	}

	std::string getWinPercent_item(uint32 wins, uint32 losses)
	{
		uint32 totalGames = wins + losses;
		if (totalGames == 0)
			return "0%";

		std::stringstream buf;
		uint32 percentage = (wins * 100) / totalGames;
		buf << percentage << "%";
		return buf.str();
	}

	std::string raceToString_item(uint8 race)
	{
		std::string race_s = "Desconocido";
		switch (race)
		{
			case RACE_HUMAN:
				race_s = "Humano";
			break;

			case RACE_ORC:
				race_s = "Orco";
			break;

			case RACE_DWARF:
				race_s = "Enano";
			break;

			case RACE_NIGHTELF:
				race_s = "Elfo de la Noche";
			break;

			case RACE_UNDEAD_PLAYER:
				race_s = "No-Muerto";
			break;

			case RACE_TAUREN:
				race_s = "Tauren";
			break;

			case RACE_GNOME:
				race_s = "Gnomo";
			break;

			case RACE_TROLL:
				race_s = "Troll";
			break;

			case RACE_BLOODELF:
				race_s = "Elfo de Sangre";
			break;

			case RACE_DRAENEI:
				race_s = "Draenei";
			break;
		}
		return race_s;
	}

	std::string classToString_item(uint8 Class)
	{
		std::string Class_s = "Desconocido";
		switch (Class)
		{
			case CLASS_WARRIOR:
				Class_s = "|cff4e2a04Guerrero|r";
			break;

			case CLASS_PALADIN:
				Class_s = "|cfff58cbaPaladin|r";
			break;

			case CLASS_HUNTER:
				Class_s = "|cffabd473Cazador|r";
			break;

			case CLASS_ROGUE:
				Class_s = "|cfffff569Picaro|r";
			break;

			case CLASS_PRIEST:
				Class_s = "|cffffffffSacerdote|r";
			break;

			case CLASS_DEATH_KNIGHT:
				Class_s = "|cffc41f3bCaballero de la Muerte|r";
			break;

			case CLASS_SHAMAN:
			   Class_s = "|cff0070deChaman|r";
			break;

			case CLASS_MAGE:
				Class_s = "|cff69ccf0Mago|r";
			break;

			case CLASS_WARLOCK:
				Class_s = "|cff9482c9Brujo|r";
			break;

			case CLASS_DRUID:
				Class_s = "|cffff7d0aDruida|r";
			break;
		}
		return Class_s;
	}

	void SendTeamInfo(Player* player, Item* item, std::string code)
	{
		ArenaTeam* team = sArenaTeamMgr->GetArenaTeamByName(code);
		if(!team)
		{
			player->GetSession()->SendNotification("Ese equipo de arenas no existe.");
			player->PlayerTalkClass->SendCloseGossip();
			return;
		}

		uint32 teamId = team->GetId();

		QueryResult result = CharacterDatabase.PQuery("SELECT name, rating, seasonWins, seasonGames - seasonWins, weekWins, weekGames - weekWins, rank, captainGuid, type FROM arena_team WHERE arenaTeamId = '%u'", teamId);

		if(!result)
		{
			player->GetSession()->SendNotification("Equipo de arenas no encontrado");
			player->PlayerTalkClass->SendCloseGossip();
			return;
		}

		Field *fields = result->Fetch();
		std::string name = fields[0].GetString();
		uint32 rating = fields[1].GetUInt32();
		uint32 seasonWins = fields[2].GetUInt32();
		uint32 seasonLosses = fields[3].GetUInt32();
		uint32 weekWins = fields[4].GetUInt32();
		uint32 weekLosses = fields[5].GetUInt32();
		uint32 rank = fields[6].GetUInt32();
		uint32 captainGuid = fields[7].GetUInt32();
		uint32 type = fields[8].GetUInt32();

		std::string seasonWinPercentage = getWinPercent_item(seasonWins, seasonLosses);
		std::string weekWinPercentage = getWinPercent_item(weekWins, weekLosses);

		std::stringstream buf;
		buf << "Nombre del equipo: " << "|cffff8000" << name << "|r";
		player->ADD_GOSSIP_ITEM(7, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
		buf.str("");

		buf << "Índice: " << "|cffe30000" << rating << "|r" << " (Rango: " << "|cffe30000" << rank << "|r" << ", Tipo: " << "|cffe30000" << type << "c" << type << "|r"")";
		player->ADD_GOSSIP_ITEM(4, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
		buf.str("");

		buf << "Total de la Semana: " << "|cffe30000" << weekWins << "-" << weekLosses << "|r"" (" << "|cffe30000" << weekWinPercentage << "|r" << "victorias)";
		player->ADD_GOSSIP_ITEM(4, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
		buf.str("");

		buf << "Total de la Temporada: " << "|cffe30000" << seasonWins << "-" << seasonLosses << "|r" << " (" << "|cffe30000" << seasonWinPercentage << "|r" << "victorias)";
		player->ADD_GOSSIP_ITEM(4, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);

		QueryResult members = CharacterDatabase.PQuery("SELECT  a.guid, a.personalRating, a.weekWins, a.weekGames - a.weekWins, a.seasonWins, a.seasonGames - a.seasonWins, c.name, c.race, c.class, c.level FROM arena_team_member a LEFT JOIN characters c ON c.guid = a.guid WHERE arenaTeamId = '%u' ORDER BY a.guid = '%u' DESC, a.seasonGames DESC, c.name ASC", teamId, captainGuid);
		if(!members)
		{
			player->ADD_GOSSIP_ITEM(7, "No se encuentran miembros en el equipo", GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
		}
		else
		{
			uint32 memberPos = 1;
			uint32 memberCount = members->GetRowCount();
			uint32 guid, personalRating, level;
			std::string name, race, Class;

			buf.str("");
			buf << "----- " << memberCount << ((memberCount == 1) ? " miembro" : " miembros") << ((memberCount == 1) ? " encontrado" : " encontrados") << " -----";
			player->ADD_GOSSIP_ITEM(0, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
			do
			{
				fields = members->Fetch();
				guid = fields[0].GetUInt32();
				personalRating = fields[1].GetUInt32();
				weekWins= fields[2].GetUInt32();
				weekLosses = fields[3].GetUInt32();
				seasonWins = fields[4].GetUInt32();
				seasonLosses = fields[5].GetUInt32();
				name = fields[6].GetString();
				race = raceToString_item(fields[7].GetUInt8());
				Class = classToString_item(fields[8].GetUInt8());
				level = fields[9].GetUInt32();

				seasonWinPercentage = getWinPercent_item(seasonWins, seasonLosses);
				weekWinPercentage = getWinPercent_item(weekWins, weekLosses);

				buf.str("");
				buf << memberPos << ". ";
				if (guid == captainGuid)
					buf <<  "Capitan del Equipo: ";

				buf << name << ", " << getPlayerStatus_item(guid);
				player->ADD_GOSSIP_ITEM(7, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
				buf.str("");

				buf << race << " " << Class << ", " << "|cffe30000" << personalRating << "|r" << " de índice personal";
				player->ADD_GOSSIP_ITEM(4, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
				buf.str("");

				buf << "Semana: " << "|cffe30000" << weekWins << "-" << weekLosses << "|r" << " (" << "|cffe30000" << weekWinPercentage << "|r" << "victorias), " << "|cffe30000" << (weekWins + weekLosses) << "|r" << " jugadas";
				player->ADD_GOSSIP_ITEM(4, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
				buf.str("");

				buf << "Temporada: " << "|cffe30000" << seasonWins << "-" << seasonLosses << "|r" << " (" << "|cffe30000" << seasonWinPercentage << "|r" << "victorias), " << "|cffe30000" << (seasonWins + seasonLosses) << "|r" << " jugadas";
				player->ADD_GOSSIP_ITEM(4, buf.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
				memberPos++;
			}while(members->NextRow());
		}
		player->SEND_GOSSIP_MENU(1, item->GetGUID());
	}

	void SendSoloInfo(Player* player, Item* item, uint32 teamId, uint64 target, uint32 type)
	{
		std::ostringstream info;
		info << "------------ " << "Tipo: " << "|cffe30000" << GetTypeAsString_item(type) << "|R" << " ------------";
		player->ADD_GOSSIP_ITEM(0, info.str().c_str(), GOSSIP_SENDER_MAIN, 0);
		info.str("");
		info.clear();
		ArenaTeam* team = sArenaTeamMgr->GetArenaTeamById(teamId);
		info << "Estadísticas: \n";
		for(ArenaTeam::MemberList::iterator itr = team->m_membersBegin(); itr != team->m_membersEnd(); ++itr)
		{
			if(target == itr->Guid)
			{
				std::ostringstream ss;

				std::string name = itr->Name;
				std::string teamName = team->GetName();
				uint32 MMR = itr->MatchMakerRating;
				uint32 personalRating = itr->PersonalRating;

				uint32 seasonGames = itr->SeasonGames;
				uint32 seasonWins = itr->SeasonWins;
				uint32 seasonLosses = seasonGames - seasonWins;

				uint32 weekGames = itr->WeekGames;
				uint32 weekWins = itr->WeekWins;
				uint32 weekLosses = weekGames - weekWins;

				ss.str("");
				ss << name << ", " << getPlayerStatus_item(itr->Guid);
				player->ADD_GOSSIP_ITEM(7, ss.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
				ss.str("");

				ss << "MMR Personal: " << "|cffe30000" << (uint32)MMR << "|R";
				player->ADD_GOSSIP_ITEM(4, ss.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
				ss.str("");

				ss << "Nombre del Equipo: " << "|cffe30000" << teamName << "|R";
				player->ADD_GOSSIP_ITEM(4, ss.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
				ss.str("");

				ss << "Índice Personal: " << "|cffe30000" << (uint32)personalRating << "|R";
				player->ADD_GOSSIP_ITEM(4, ss.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
				ss.str("");

				ss << "Semana: " << "|cffe30000" << (uint32)weekWins << "|R" << "-" << "|cffe30000" << (uint32)weekLosses << "|R" << "," << "|cffe30000 " << (uint32)weekGames << "|R" << " jugadas en total";
				player->ADD_GOSSIP_ITEM(4, ss.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
				ss.str("");

				ss << "Temporada: " << "|cffe30000" << (uint32)seasonWins << "|R" << "-" << "|cffe30000" << (uint32)seasonLosses << "|R" << "," << "|cffe30000 " << (uint32)seasonGames << "|R" << " jugadas en total";
				player->ADD_GOSSIP_ITEM(4, ss.str(), GOSSIP_SENDER_MAIN, ARENA_GOODBYE);
				ss.str("");
			}
		}
		player->SEND_GOSSIP_MENU(907, item->GetGUID());
	}

	void SendOptions(Player* player, Item* item, const char* code)
	{
		uint64 Guid;
		Player* target = sObjectAccessor->FindPlayerByName(code);

		if(!target || !target->GetSession())
			Guid = GetGuidByDb_item(code);
		else
			Guid = target->GetGUID();

		if(!Guid)
		{
			player->GetSession()->SendNotification("El jugador no existe.");
			player->PlayerTalkClass->SendCloseGossip();
			return;
		}

		if(!GetArenaTeamId_item(Guid, 2) && !GetArenaTeamId_item(Guid, 3) && !GetArenaTeamId_item(Guid, 5))
		{
			player->GetSession()->SendNotification("El jugador no está en un Equipo de Arenas.");
			player->PlayerTalkClass->SendCloseGossip();
			return;
		}

		for(int i = 2; i < 6; ++i)
		{
			if(i == 4)
				continue;

			uint16 action = (i != 5 ? i*2-(i == 3 ? 1 : 0) : i+1);

			if(GetArenaTeamId_item(Guid, i))
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GetTypeAsString_item(i), Guid, action+1700);
		}
		player->SEND_GOSSIP_MENU(1, item->GetGUID());
	}


	uint32 Jugadores_item()
	{
		QueryResult contar_area = CharacterDatabase.PQuery("SELECT COUNT(area) FROM characters WHERE area = 368");

		if (!contar_area)
			return 0;
		else
		{
			Field* fields = contar_area->Fetch();
				return fields[0].GetUInt32();
		}
	}

/*COMPRESION 2*/

	bool PlayerAlreadyHasTwoProfessions(const Player *player)
	{
		uint32 skillCount = 0;

            if (player->HasSkill(SKILL_MINING))
                skillCount++;

            if (player->HasSkill(SKILL_SKINNING))
                skillCount++;

            if (player->HasSkill(SKILL_HERBALISM))
                skillCount++;

            if (skillCount >= sWorld->getIntConfig(CONFIG_MAX_PRIMARY_TRADE_SKILL))
                return true;

            for (uint32 i = 1; i < sSkillLineStore.GetNumRows(); ++i)
            {
                SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(i);
                if (!SkillInfo)
                    continue;

                if (SkillInfo->categoryId == SKILL_CATEGORY_SECONDARY)
                    continue;

                if ((SkillInfo->categoryId != SKILL_CATEGORY_PROFESSION) || !SkillInfo->canLink)
                    continue;

                const uint32 skillID = SkillInfo->id;
                if (player->HasSkill(skillID))
                    skillCount++;

				if (skillCount >= sWorld->getIntConfig(CONFIG_MAX_PRIMARY_TRADE_SKILL))
					return true;
            }
            return false;
	}

	bool LearnAllRecipesInProfession(Player *player, SkillType skill)
	{
		ChatHandler handler(player);
		char* skill_name;

		SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(skill);
		skill_name = SkillInfo->name[handler.GetSessionDbcLocale()];

		if (!SkillInfo)
            {
				sLog->outError("Teleport NPC: received non-valid skill ID (LearnAllRecipesInProfession)");
				return false;
            }

		LearnSkillRecipesHelper(player, SkillInfo->id);

		uint16 maxLevel = player->GetPureMaxSkillValue(SkillInfo->id);
		player->SetSkill(SkillInfo->id, player->GetSkillStep(SkillInfo->id), maxLevel, maxLevel);
		handler.PSendSysMessage(LANG_COMMAND_LEARN_ALL_RECIPES, skill_name);

		return true;
	}

	void LearnSkillRecipesHelper(Player *player, uint32 skill_id)
	{
		uint32 classmask = player->getClassMask();

		for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
			{
				SkillLineAbilityEntry const *skillLine = sSkillLineAbilityStore.LookupEntry(j);
				if (!skillLine)
					continue;

				// wrong skill
				if (skillLine->skillId != skill_id)
					continue;

				// not high rank
				if (skillLine->forward_spellid)
					continue;

				// skip racial skills
				if (skillLine->racemask != 0)
					continue;

				// skip wrong class skills
				if (skillLine->classmask && (skillLine->classmask & classmask) == 0)
					continue;

				SpellEntry const* spellInfo = sSpellStore.LookupEntry(skillLine->spellId);
				if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo, player, false))
					continue;

				player->learnSpell(skillLine->spellId, false);
			}
	}

	bool IsSecondarySkill(SkillType skill)
	{
		return skill == SKILL_COOKING || skill == SKILL_FIRST_AID || skill == SKILL_FISHING;
	}

	void CompleteLearnProfession(Player* player, Item* item, SkillType skill)
	{
		if (PlayerAlreadyHasTwoProfessions(player) && !IsSecondarySkill(skill))
			ChatHandler(player->GetSession()).PSendSysMessage("""\xc2\xa1""Ya sabes dos profesiones!");
		else
			{
				if (!LearnAllRecipesInProfession(player, skill))
					ChatHandler(player->GetSession()).PSendSysMessage("""\xc2\xa1""Error interno!");
			}
	}

	bool isEquiped(Player* player, uint8 slot)
	{
		Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);

		if (!item)
			return false;

		return true;
	}

	bool validItem(uint8 type, Item* item)
	{
		ItemTemplate const* pProto = item->GetTemplate();

		// Una Mano
		if (type == 1 && (pProto->Class == 2 && (pProto->SubClass == 0 || pProto->SubClass == 1 || pProto->SubClass == 4 || pProto->SubClass == 5 || pProto->SubClass == 6 || pProto->SubClass == 7 || pProto->SubClass == 8 || pProto->SubClass == 10 || pProto->SubClass == 13 || pProto->SubClass == 15)))
			return true;
		// Dos Manos
		if (type == 2 && (pProto->Class == 2 && (pProto->SubClass == 1 || pProto->SubClass == 5 || pProto->SubClass == 6 || pProto->SubClass == 8 || pProto->SubClass == 10)))
			return true;
		// Baston
		if (type == 3 && (pProto->Class == 2 && pProto->SubClass == 10))
			return true;
		// Escudo
		if (type == 4 && pProto->InventoryType == 14)
			return true;

		return false;
	}

	void Enchant(Player* player, Item* item, uint8 slot, uint32 enchant, uint8 type)
	{
		item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);

		if (!item || (type != 0 && !validItem(type, item)))
			{
				player->CLOSE_GOSSIP_MENU();
				player->GetSession()->SendNotification("|cFF00FFFF""\xc2\xbf""Seguro que tienes ese objeto equipado?|r");
				return;
			}

		SpellItemEnchantmentEntry const* enchantid = sSpellItemEnchantmentStore.LookupEntry(enchant);

		if (!enchantid)
			return;

		player->ApplyEnchantment(item, PERM_ENCHANTMENT_SLOT, false);
		item->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchant, 0, 0);
		player->ApplyEnchantment(item, PERM_ENCHANTMENT_SLOT, true);
		//player->GetSession()->SendNotification("|cffFF0000""\xc2\xa1""|cFFFFD700%s |cffFF0000encantado correctamente!", item->GetTemplate()->Name1.c_str());
		std::string color = "|cff";

		switch (item->GetTemplate()->Quality)
			{
				case 0: color += "9d9d9d"; break;
				case 1: color += "ffffff"; break;
				case 2: color += "1eff00"; break;
				case 3: color += "0070dd"; break;
				case 4: color += "a335ee"; break;
				case 5: color += "ff8000"; break;
			}

			ChatHandler(player->GetSession()).PSendSysMessage("|cffFF0000""\xc2\xa1""%s[%s] |cffFF0000encantado correctamente!", color.c_str(), item->GetTemplate()->Name1.c_str());
	}

	std::string GetClassNameById(uint8 id)
	{
		std::string sClass = "";

            switch (id)
            {
                case CLASS_WARRIOR:         sClass = "Guerrero ";        break;
                case CLASS_PALADIN:         sClass = "Paladin ";           break;
                case CLASS_HUNTER:          sClass = "Cazador ";           break;
                case CLASS_ROGUE:           sClass = "Picaro ";          break;
                case CLASS_PRIEST:          sClass = "Sacerdote ";         break;
                case CLASS_DEATH_KNIGHT:    sClass = "DK ";             break;
                case CLASS_SHAMAN:          sClass = "Chaman ";          break;
                case CLASS_MAGE:            sClass = "Mago ";           break;
                case CLASS_WARLOCK:         sClass = "Brujo ";        break;
                case CLASS_DRUID:           sClass = "Druida ";          break;
            }

            return sClass;
	}

	std::string GetGamesStringData(Battleground *arena, uint16 mmr)
	{
		std::string teamsMember[BG_TEAMS_COUNT];
		uint32 firstTeamId = 0;

            for (Battleground::BattlegroundPlayerMap::const_iterator itr = arena->GetPlayers().begin(); itr != arena->GetPlayers().end(); ++itr)
                if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                {
                    if (player->isSpectator())
                        continue;

                    uint32 team = itr->second.Team;
                    if (!firstTeamId)
                        firstTeamId = team;

                    teamsMember[firstTeamId == team] += GetClassNameById(player->getClass());
                }
			std::string data = teamsMember[0] +" - ";
            data += teamsMember[1];
            return data;
	}

	uint64 GetFirstPlayerGuid(Battleground *arena)
	{
		for (Battleground::BattlegroundPlayerMap::const_iterator itr = arena->GetPlayers().begin(); itr != arena->GetPlayers().end(); ++itr)
			if (Player* player = ObjectAccessor::FindPlayer(itr->first))
				return itr->first;
		return 0;
	}

	void ShowPage(Player *player, uint16 page, bool isTop)
	{
            /*uint16 highGames  = 0;
            uint16 lowGames   = 0;*/
			uint16 TypeTwo = 0;
            uint16 TypeThree = 0;
            bool haveNextPage = false;
            for (uint8 i = 0; i <= MAX_BATTLEGROUND_TYPE_ID; ++i)
            {
                if (!sBattlegroundMgr->IsArenaType(BattlegroundTypeId(i)))
                    continue;

                BattlegroundSet bgs = sBattlegroundMgr->GetBattlegroundsByType(BattlegroundTypeId(i));

				for(BattlegroundSet::iterator itr = bgs.begin(); itr != bgs.end(); ++itr)
				{
                    Battleground* arena = itr->second;

                    if (!arena->GetPlayersSize())
                        continue;

					uint16 mmrTwo = arena->GetArenaMatchmakerRating(0);
					uint16 mmrThree = arena->GetArenaMatchmakerRating(1);

                    if (isTop && arena->GetArenaType() == ARENA_TYPE_3v3)
                    {
						TypeThree++;
						if(TypeThree > (page + 1) * GamesOnPage)
                        {
                            haveNextPage = true;
                            break;
                        }

						player->ADD_GOSSIP_ITEM(0, "Arenas 3c3 disput""\xC3\xA1""ndose actualmente:", GOSSIP_SENDER_MAIN, 2000);
						if(TypeThree >= page * GamesOnPage)
                           player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GetGamesStringData(arena, mmrThree), GOSSIP_SENDER_MAIN, 3000 + GetFirstPlayerGuid(arena));
                    }
                    else if (!isTop && arena->GetArenaType() == ARENA_TYPE_2v2)
                    {
						TypeTwo++;
                        if(TypeTwo > (page + 1) * GamesOnPage)
                        {
                            haveNextPage = true;
                            break;
                        }

						player->ADD_GOSSIP_ITEM(0, "Arenas 2c2 disput""\xC3\xA1""ndose actualmente:", GOSSIP_SENDER_MAIN, 1000);
						if(TypeTwo >= page * GamesOnPage)
                            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, GetGamesStringData(arena, mmrTwo), GOSSIP_SENDER_MAIN, 3000 + GetFirstPlayerGuid(arena));
					}
                }
            }

            /*if (page > 0)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cff9b0000<- Anterior", GOSSIP_SENDER_MAIN, 1000 + page - 1);*/

            if (haveNextPage)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "|cff2e2efeSiguiente ->", GOSSIP_SENDER_MAIN, 1000 + page + 1);
	}

	void CreatePet(Player *player, Item * item, uint32 entry)
	{
			if(player->getClass() != CLASS_HUNTER) {
				player->GetSession()->SendAreaTriggerMessage("""\xc2\xa1""No eres un cazador!", player->GetGUID());
				player->PlayerTalkClass->SendCloseGossip();
				return;
			}

			if(player->GetPet()) {
				player->GetSession()->SendAreaTriggerMessage("""\xc2\xa1""Primero debes retirar a tu mascota!", player->GetGUID());
				player->PlayerTalkClass->SendCloseGossip();
				return;
			}

			TempSummon *creatureTarget = player->SummonCreature(entry, player->GetPositionX(), player->GetPositionY()+2, player->GetPositionZ(), player->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 500);
			if(!creatureTarget) return;

			Pet* pet = player->CreateTamedPetFrom(creatureTarget, 0);
			if(!pet) return;

			// Matamos a la criatura original
			creatureTarget->setDeathState(JUST_DIED);
			creatureTarget->RemoveCorpse();
			creatureTarget->SetHealth(0);

			pet->SetPower(POWER_HAPPINESS, 1048000);

			//pet->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE,0);
			//pet->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, uint32((Trinity::XP::xp_to_level(70))/4));

			// Preparamos el efecto visual para la subida de nivel
			pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->getLevel() - 1);
			pet->GetMap()->Add((Creature*)pet);//MYTH
			// Efecto visual para la subida de nivel
			pet->SetUInt32Value(UNIT_FIELD_LEVEL, player->getLevel());


			if(!pet->InitStatsForLevel(player->getLevel()))
				//sLog->outError (LOG_FILTER_GENERAL,"Fallo en la creacion de la mascota: no existen estadísticas para el entry %u", entry);//TRINITY
				sLog->outError ("Fallo en la creacion de la mascota: no existen estadisticas para el entry %u", entry);//MYTH

			pet->UpdateAllStats();

			// El usuario ya tiene la mascota
			player->SetMinion(pet, true);

			pet->SavePetToDB(PET_SAVE_AS_CURRENT);
			pet->InitTalentForLevel();
			player->PetSpellInitialize();

			//Finalizado el proceso y la mascota ya ha sido entregada
			player->PlayerTalkClass->SendCloseGossip();
	}

	uint32 optionToTeamType(uint32 option)
	{
		uint32 teamType;

		switch(option)
			{
				case ARENA_2V2_LADDER:
					teamType = 2;
				break;
			}

		return teamType;
	}

	uint32 teamTypeToOption(uint32 teamType)
		{
			uint32 option;
			switch(teamType)
			{
				case 2:
					option = ARENA_2V2_LADDER;
				break;
			}
			return option;
		}
