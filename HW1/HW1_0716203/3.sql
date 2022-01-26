select Ch.champion_name as champion_name, COUNT(Ch.champion_id) as cnt 
from participant P, champ Ch 
where Ch.champion_id = P.champion_id and P.position = 'JUNGLE'
group by champion_name
order by cnt DESC limit 3;
