SELECT
    FLOOR(total_spent / 50) * 50 AS monetary_range,
    COUNT(*) AS num_customers
FROM (
    SELECT
        user_id,
        SUM(price) AS total_spent
    FROM customers
    WHERE event_type = 'purchase'
    GROUP BY user_id
) sub
GROUP BY monetary_range
ORDER BY monetary_range;