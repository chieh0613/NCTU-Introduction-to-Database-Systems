select temp.v as version, sum(temp.win) as win_cnt, count(temp.win)-sum(temp.win) as lose_cnt, sum(temp.win)/(sum(temp.win)+count(temp.win)-sum(temp.win)) as win_ratio
from (select distinct p1.player_id, s.win as win, p.match_id , SUBSTRING_INDEX(m.version,".",2) as v
	from participant p1, participant p2 ,participant p, match_info m, stat s
	where p1.champion_id = 17 and p2.champion_id = 64 and p1.player div 6 = p2.player div 6 
      	and p.match_id = p1.match_id and p.match_id = p2.match_id  and m.match_id = p.match_id
      	and p1.player_id = s.player_id
	order by v) as temp
group by temp.v
order by temp.v;

