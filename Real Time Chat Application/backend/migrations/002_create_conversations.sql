CREATE TABLE conversations (
    id         UUID        PRIMARY KEY DEFAULT uuid_generate_v4(),
    group_id   UUID,
    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);

CREATE TABLE conversation_participants (
    conversation_id UUID NOT NULL REFERENCES conversations(id) ON DELETE CASCADE,
    user_id         UUID NOT NULL REFERENCES users(id)         ON DELETE CASCADE,
    PRIMARY KEY (conversation_id, user_id)
);
