select M.match_id as match_id, SEC_TO_TIME(M.duration) as time
from match_info M
order by time DESC limit 5;
