select ch.champion_name as champion_name
from champ ch
where ch.champion_id NOT IN (select distinct ban.champion_id from teamban ban where ban.match_id IN (select distinct m.match_id from match_info m where m.version like '7.7.%') )
order by champion_name ASC;

