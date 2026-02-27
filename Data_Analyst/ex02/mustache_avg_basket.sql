SELECT
    user_id,
    AVG(price) AS avg_basket
FROM customers
WHERE event_type = 'purchase'
GROUP BY user_id;