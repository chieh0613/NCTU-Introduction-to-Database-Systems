select 'lose' as win_lose, count(temp.l) as cnt
from (select p.match_id as match_id, AVG(s.longesttimespentliving) as l
	      from participant p, stat s
	      where  p.player_id = s.player_id and s.win = 0
	      group by match_id
	      having l >= 1200) as temp
union
select 'win' as win_lose, count(temp.l) as cnt
from (select p.match_id as match_id, AVG(s.longesttimespentliving) as l
	from participant p, stat s
	where  p.player_id = s.player_id and s.win = 1
	group by match_id
	having l >= 1200) as temp;
