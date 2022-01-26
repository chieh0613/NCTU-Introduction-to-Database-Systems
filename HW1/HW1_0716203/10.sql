select temp2.name as self_champion_name, temp2.c_win_ratio as win_ratio, temp2.c_kda as self_kda, temp2.c_avg_gold as self_avg_gold, ch.champion_name as enemy_champ_name, temp2.r_kda as enemy_kda, temp2.r_avg_gold as enemy_avg_gold, temp2.num as battle_record
from champ ch, (select temp.champion as name, count(temp.champion) as num,SUM(temp.c_win)/count(temp.c_win) as c_win_ratio, (sum(temp.c_assist)+sum(temp.c_kill))/sum(temp.c_death) as c_kda, (sum(temp.r_assist)+sum(temp.r_kill))/sum(temp.r_death) as r_kda, avg(temp.r_earned) as r_avg_gold, avg(temp.c_earned) as c_avg_gold
from (select distinct  ch.champion_name as champion, s2.win as c_win, s1.deaths as r_death, s1.kills as r_kill, s1.assists as r_assist, s2.deaths as c_death, s2.kills as c_kill, s2.assists as c_assist, s1.goldearned as r_earned, s2.goldearned as c_earned 
from participant p1, participant p2, participant p, stat s1, stat s2, champ ch
where p1.champion_id = 58 and p1.position = 'TOP'and p2.position = 'TOP' and  p1.player div 6 <> p2.player div 6 and p.match_id = p1.match_id and p.match_id = p2.match_id and p1.player_id = s1.player_id and p2.player_id = s2.player_id and ch.champion_id = p2.champion_id ) as temp
group by temp.champion
having sum(temp.c_death) > 0) as temp2
where ch.champion_id = 58 and temp2.num > 100
order by win_ratio DESC limit 5;
