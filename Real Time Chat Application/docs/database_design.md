# database_design.md

## PostgreSQL Schema (Core)

**Users**
- id (UUID, PK)
- username (VARCHAR)

**Groups**
- id (UUID, PK)
- name (VARCHAR)

**Group_Members (The Edge List)**
- group_id (UUID, FK)
- user_id (UUID, FK)
- PRIMARY KEY (group_id, user_id)
- *Index on user_id for fast lookup of a user's groups.*

**Messages**
- id (UUID, PK)
- group_id (UUID, FK)
- sender_id (UUID, FK)
- content (TEXT)
- server_timestamp (TIMESTAMP)
- *Index on (group_id, server_timestamp) for rapid history retrieval.*