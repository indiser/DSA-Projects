CREATE TABLE messages (
    id              UUID        PRIMARY KEY DEFAULT uuid_generate_v4(),
    conversation_id UUID        NOT NULL REFERENCES conversations(id) ON DELETE CASCADE,
    sender_id       UUID        NOT NULL REFERENCES users(id)         ON DELETE CASCADE,
    content         TEXT        NOT NULL,
    delivered       BOOLEAN     NOT NULL DEFAULT FALSE,
    read            BOOLEAN     NOT NULL DEFAULT FALSE,
    sequence_id     BIGINT      NOT NULL DEFAULT 0,
    created_at      TIMESTAMPTZ NOT NULL DEFAULT NOW()
);
