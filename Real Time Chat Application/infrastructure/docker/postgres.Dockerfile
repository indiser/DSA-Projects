FROM postgres:16-alpine

# Migrations are mounted via docker-compose into /docker-entrypoint-initdb.d
# and run automatically on first container start in filename order.

EXPOSE 5432
