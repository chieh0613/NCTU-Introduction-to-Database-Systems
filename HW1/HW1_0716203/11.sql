select sum(temp.FI) as win_num_of_FI, sum(temp.FT) as win_num_of_FT
from (select s1.win as FI, s2.win as FT
from participant p1, participant p2, stat s1, stat s2
where p1.position = 'TOP' and p2.position = 'TOP' and ((p1.ss1 = 'Flash' and p1.ss2 = 'Ignite') or p1.ss1 = 'Ignite' and p1.ss2 = 'Flash') and ((p2.ss1 = 'Flash' and p2.ss2 = 'Teleport') or p2.ss1 = 'Teleport' and p2.ss2 = 'Flash')  and p1.player_id = s1.player_id and p2.player_id = s2.player_id
and p1.match_id = p2.match_id and p1.player div 6 <> p2.player div 6) as temp;
