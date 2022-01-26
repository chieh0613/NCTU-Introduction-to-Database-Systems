SELECT COUNT(DISTINCT SUBSTRING_INDEX(version,".",2)) as cnt
FROM match_info; 

