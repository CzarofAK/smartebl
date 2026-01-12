# Samba Share Setup

This script sets up a Samba share for the `/smartebl` directory.

## Installation Script

Save and run the following script with sudo:

```bash
#!/bin/bash

set -e

SHARE_PATH="/smartebl"
SHARE_NAME="smartebl"
CURRENT_USER="${SUDO_USER:-$USER}"

echo "=== Samba Share Setup ==="

# Install Samba
echo "[1/5] Installing Samba..."
apt update
apt install -y samba

# Backup existing config
echo "[2/5] Backing up existing configuration..."
cp /etc/samba/smb.conf /etc/samba/smb.conf.backup.$(date +%Y%m%d%H%M%S)

# Add share configuration
echo "[3/5] Configuring Samba share..."
if ! grep -q "\[$SHARE_NAME\]" /etc/samba/smb.conf; then
    cat >> /etc/samba/smb.conf << EOF

[$SHARE_NAME]
   path = $SHARE_PATH
   browseable = yes
   read only = no
   guest ok = no
   valid users = $CURRENT_USER
   create mask = 0664
   directory mask = 0775
   force user = $CURRENT_USER
EOF
    echo "Share configuration added."
else
    echo "Share already exists in config."
fi

# Set directory permissions
echo "[4/5] Setting directory permissions..."
chown -R $CURRENT_USER:$CURRENT_USER "$SHARE_PATH"
chmod -R 775 "$SHARE_PATH"

# Restart Samba services
echo "[5/5] Restarting Samba services..."
systemctl restart smbd nmbd
systemctl enable smbd nmbd

echo ""
echo "=== Setup Complete ==="
echo ""
echo "Next steps:"
echo "1. Set your Samba password by running:"
echo "   sudo smbpasswd -a $CURRENT_USER"
echo ""
echo "2. Access the share from:"
echo "   - Windows: \\\\$(hostname -I | awk '{print $1}')\\$SHARE_NAME"
echo "   - Linux:   smb://$(hostname -I | awk '{print $1}')/$SHARE_NAME"
echo ""
```

## Quick Install

Run these commands:

```bash
# Create the script
sudo nano /tmp/setup-samba.sh
# Paste the script above, save and exit

# Make it executable and run
sudo chmod +x /tmp/setup-samba.sh
sudo /tmp/setup-samba.sh

# Set your Samba password
sudo smbpasswd -a $USER
```

## Firewall Configuration

If you have a firewall enabled, allow Samba traffic:

```bash
sudo ufw allow samba
```

## Troubleshooting

Check Samba status:
```bash
sudo systemctl status smbd
```

Test configuration:
```bash
testparm
```

List shares:
```bash
smbclient -L localhost -U $USER
```
