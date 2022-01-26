select t1.win as win_num, t2.cnt as total_num, t1.win/t2.cnt as win_ratio
from (select count(distinct p1.match_id) as win 
      from stat s, participant p1
      where s.firstblood = 1 and s.win = 1 and s.player_id = p1.player_id) as t1, 
     (select count(m.match_id) as cnt from match_info m) as t2;


select t1.win as win_num, t2.cnt as total_num, t1.win/t2.cnt as win_ratio , p.position as position
from participant p, (select count(distinct p.player_id) as win, p.position as pos1
from stat s, participant p 
where s.firstblood = 1 and s.win = 1 and s.player_id = p.player_id 
group by p.position) as t1, (select count(p1.match_id) as cnt, p1.position as pos2
from participant p1
group by p1.position) as t2
where p.position = t2.pos2 and p.position = t1.pos1
group by p.position
order by win_ratio DESC;


