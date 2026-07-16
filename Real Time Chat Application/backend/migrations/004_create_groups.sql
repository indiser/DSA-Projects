CREATE TABLE groups (
    id         UUID        PRIMARY KEY DEFAULT uuid_generate_v4(),
    name       VARCHAR(100) NOT NULL,
    owner_id   UUID        NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

ALTER TABLE conversations
    ADD CONSTRAINT fk_conversations_group
    FOREIGN KEY (group_id) REFERENCES groups(id) ON DELETE SET NULL;
