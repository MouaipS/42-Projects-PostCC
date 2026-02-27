SELECT
    DATE(event_time) AS day,
    ROUND(SUM(price)::numeric / COUNT(DISTINCT user_id), 2) AS avg_spend
FROM customers
WHERE event_type = 'purchase'
  AND event_time >= '2022-10-01'
  AND event_time < '2023-03-01'
GROUP BY day
ORDER BY day;