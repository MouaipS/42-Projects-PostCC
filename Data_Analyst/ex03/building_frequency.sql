SELECT
    order_count AS frequency,
    COUNT(*) AS num_customers
FROM (
    SELECT
        user_id,
        COUNT(*) AS order_count
    FROM customers
    WHERE event_type = 'purchase'
    GROUP BY user_id
) sub
GROUP BY order_count
ORDER BY frequency;