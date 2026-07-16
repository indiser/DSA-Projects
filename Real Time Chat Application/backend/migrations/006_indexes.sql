-- users
CREATE INDEX idx_users_email    ON users(email);
CREATE INDEX idx_users_username ON users(username);

-- conversations
CREATE INDEX idx_conv_participants_user ON conversation_participants(user_id);

-- messages: primary query path — history for a conversation ordered by time
CREATE INDEX idx_messages_conv_created  ON messages(conversation_id, created_at DESC);
CREATE INDEX idx_messages_sender        ON messages(sender_id);
CREATE INDEX idx_messages_undelivered   ON messages(conversation_id) WHERE delivered = FALSE;

-- full-text search on message content
ALTER TABLE messages ADD COLUMN content_tsv TSVECTOR
    GENERATED ALWAYS AS (to_tsvector('english', content)) STORED;
CREATE INDEX idx_messages_fts ON messages USING GIN(content_tsv);

-- groups
CREATE INDEX idx_groups_owner ON groups(owner_id);

-- group_members: fast lookup of all groups a user belongs to
CREATE INDEX idx_group_members_user ON group_members(user_id);
