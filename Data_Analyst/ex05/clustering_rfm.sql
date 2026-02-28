SELECT
    user_id,
    EXTRACT(MONTH FROM AGE(
        (SELECT MAX(event_time) FROM customers WHERE event_type = 'purchase'),
        MAX(event_time)
    )) AS recency,
    COUNT(*) AS frequency,
    SUM(price) AS monetary
FROM customers
WHERE event_type = 'purchase'
GROUP BY user_id;