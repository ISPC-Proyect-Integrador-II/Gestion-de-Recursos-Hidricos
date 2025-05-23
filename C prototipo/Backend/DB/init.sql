-- =======================================
-- 1) Crear la BD y la tabla device_info
-- =======================================
CREATE DATABASE IF NOT EXISTS hidroponia;
USE hidroponia;

CREATE TABLE IF NOT EXISTS device_info (
  device_id   VARCHAR(64)  NOT NULL PRIMARY KEY,
  name        VARCHAR(128) NOT NULL,
  created_at  DATETIME     NOT NULL DEFAULT CURRENT_TIMESTAMP,
  transmitter TINYINT(1)   NOT NULL DEFAULT 0,
  receiver    TINYINT(1)   NOT NULL DEFAULT 0,
  gateway     TINYINT(1)   NOT NULL DEFAULT 0
) ENGINE=InnoDB CHARSET=utf8mb4;

-- =======================================
-- 2) Usuarios y permisos
-- =======================================

-- Admin: todos los permisos sobre 'hidroponia', salvo DROP DATABASE
CREATE USER IF NOT EXISTS 'admin'@'%' IDENTIFIED BY 'admin';
GRANT ALL ON hidroponia.* TO 'admin'@'%';

-- Dispositivo: solo INSERT y UPDATE en device_info
CREATE USER IF NOT EXISTS 'device'@'%' IDENTIFIED BY 'devicepassword';
GRANT INSERT, UPDATE ON hidroponia.device_info TO 'device'@'%';

-- Usuario de aplicación: SELECT, INSERT, UPDATE, DELETE
CREATE USER IF NOT EXISTS 'user'@'%' IDENTIFIED BY 'userpassword';
GRANT SELECT, INSERT, UPDATE, DELETE ON hidroponia.* TO 'user'@'%';

FLUSH PRIVILEGES;
