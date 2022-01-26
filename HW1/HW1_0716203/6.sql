select 'DUO_CARRY' as position,ch.champion_name as champion_name
from champ ch, (select p.champion_id as id,count(ch.champion_name) as cnt
	from participant p,champ ch, match_info m
	where  m.match_id = p.match_id and p.champion_id = ch.champion_id and p.position = 'DUO_CARRY' and duration between 2400 and 3000
	group by p.champion_id
	order by cnt DESC limit 1
) as temp
where ch.champion_id = temp.id
union
select 'DUO_SUPPORT' as position,ch.champion_name as champion_name
from champ ch, (select p.champion_id as id,count(ch.champion_name) as cnt
	from participant p,champ ch, match_info m
	where  m.match_id = p.match_id and p.champion_id = ch.champion_id and p.position = 'DUO_SUPPORT' and duration between 2400 and 3000
	group by p.champion_id
	order by cnt DESC limit 1
) as temp
where ch.champion_id = temp.id
union
select 'JUNGLE' as position,ch.champion_name as champion_name
from champ ch, (select p.champion_id as id,count(ch.champion_name) as cnt
	from participant p,champ ch, match_info m
	where  m.match_id = p.match_id and p.champion_id = ch.champion_id and p.position = 'JUNGLE' and duration between 2400 and 3000
	group by p.champion_id
	order by cnt DESC limit 1
) as temp
where ch.champion_id = temp.id
union
select 'MID' as position, ch.champion_name as champion_name
from champ ch, (select p.champion_id as id,count(ch.champion_name) as cnt
	from participant p,champ ch, match_info m
	where  m.match_id = p.match_id and p.champion_id = ch.champion_id and p.position = 'MID' and duration between 2400 and 3000
	group by p.champion_id
	order by cnt DESC limit 1
) as temp
where ch.champion_id = temp.id
union
select 'TOP' as position, ch.champion_name as champion_name
from champ ch, (select p.champion_id as id,count(ch.champion_name) as cnt
	from participant p,champ ch, match_info m
	where  m.match_id = p.match_id and p.champion_id = ch.champion_id and p.position = 'TOP' and duration between 2400 and 3000
	group by p.champion_id
	order by cnt DESC limit 1
) as temp
where ch.champion_id = temp.id;
