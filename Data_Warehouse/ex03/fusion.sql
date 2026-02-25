-- Index pour accélérer le JOIN
CREATE INDEX IF NOT EXISTS idx_customers_product_id ON customers(product_id);
CREATE INDEX IF NOT EXISTS idx_items_product_id ON items(product_id);

-- Ajout des colonnes si elles n'existent pas encore
ALTER TABLE customers
    ADD COLUMN IF NOT EXISTS category_id BIGINT,
    ADD COLUMN IF NOT EXISTS category_code VARCHAR(255),
    ADD COLUMN IF NOT EXISTS brand VARCHAR(255);

-- Mise à jour directe sans table intermédiaire
UPDATE customers c
SET
    category_id   = i.category_id,
    category_code = i.category_code,
    brand         = i.brand
FROM (
    SELECT
        product_id,
        MAX(category_id)   AS category_id,
        MAX(category_code) AS category_code,
        MAX(brand)         AS brand
    FROM items
    GROUP BY product_id
) i
WHERE c.product_id = i.product_id;