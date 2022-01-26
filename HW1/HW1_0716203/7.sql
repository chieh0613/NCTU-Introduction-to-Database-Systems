select 'DUO_CARRY' as position ,ch.champion_name as name, temp.kda as kda
from champ ch, (select ch.champion_id as id, (sum(s.assists)+sum(s.kills))/sum(s.deaths) as kda
	from stat s,participant p, champ ch
	where p.position = 'DUO_CARRY'  and s.player_id = p.player_id and p.champion_id = ch.champion_id
	group by p.champion_id
	order by kda DESC limit 1) as temp
where ch.champion_id = temp.id
union
select 'DUO_SUPPORT' as position ,ch.champion_name as name, temp.kda as kda
from champ ch, (select ch.champion_id as id, (sum(s.assists)+sum(s.kills))/sum(s.deaths) as kda
	from stat s,participant p, champ ch
	where p.position = 'DUO_SUPPORT'  and s.player_id = p.player_id and p.champion_id = ch.champion_id
	group by p.champion_id
	order by kda DESC limit 1) as temp
where ch.champion_id = temp.id
union
select 'JUNGLE' as position ,ch.champion_name as name, temp.kda as kda
from champ ch, (select ch.champion_id as id, (sum(s.assists)+sum(s.kills))/sum(s.deaths) as kda
	from stat s,participant p, champ ch
	where p.position = 'JUNGLE'  and s.player_id = p.player_id and p.champion_id = ch.champion_id
	group by p.champion_id
	order by kda DESC limit 1) as temp
where ch.champion_id = temp.id
union
select 'MID' as position ,ch.champion_name as name, temp.kda as kda
from champ ch, (select ch.champion_id as id, (sum(s.assists)+sum(s.kills))/sum(s.deaths) as kda
	from stat s,participant p, champ ch
	where p.position = 'MID'  and s.player_id = p.player_id and p.champion_id = ch.champion_id
	group by p.champion_id
	order by kda DESC limit 1) as temp
where ch.champion_id = temp.id
union
select 'TOP' as position ,ch.champion_name as name, temp.kda as kda
from champ ch, (select ch.champion_id as id, (sum(s.assists)+sum(s.kills))/sum(s.deaths) as kda
	from stat s,participant p, champ ch
	where p.position = 'TOP'  and s.player_id = p.player_id and p.champion_id = ch.champion_id
	group by p.champion_id
	order by kda DESC limit 1) as temp
where ch.champion_id = temp.id;
