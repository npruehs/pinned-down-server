

#include "ServerEventWriter.h"

using namespace PinnedDownServer::Network;

int ServerEventWriter::WriteServerEvent(SOCKET client, ServerEvent event)
{
	char sendbuf[512];
	memcpy(&sendbuf, &event.eventType, sizeof(ServerEventType));
	return send(client, sendbuf, sizeof(ServerEventType), 0);
}