SELECT
    user_id,
    EXTRACT(MONTH FROM AGE(MAX(event_time), MIN(event_time))) AS recency,
    COUNT(*) AS frequency,
    SUM(price) AS monetary
FROM customers
WHERE event_type = 'purchase'
GROUP BY user_id;