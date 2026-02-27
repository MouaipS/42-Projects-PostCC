SELECT
    TO_CHAR(DATE_TRUNC('month', event_time), 'Mon') AS month,
    DATE_TRUNC('month', event_time) AS month_order,
    ROUND(SUM(price)::numeric / 1000000, 2) AS total_sales
FROM customers
WHERE event_type = 'purchase'
  AND event_time >= '2022-10-01'
  AND event_time < '2023-03-01'
GROUP BY DATE_TRUNC('month', event_time)
ORDER BY month_order;