SELECT event_type, COUNT(*) AS total
FROM customers
GROUP BY event_type
ORDER BY total DESC;