#include <memory>
#include <strstream>

#include "ServerEventWriter.h"

using namespace PinnedDownServer::Network;

int ServerEventWriter::WriteServerEvent(SOCKET client, Event& serverEvent)
{
	std::ostrstream	out;

	out << serverEvent.GetEventType().GetString() << " ";
	serverEvent.Serialize(out);
	out << "\r\n";

	auto buffer = out.rdbuf()->str();
	int count = (int)out.pcount();

	auto sendBuffer = new char[count + sizeof(int)];
	*(int*)sendBuffer = count;
	memcpy(sendBuffer + sizeof(int), buffer, count);

	int result = send(client, sendBuffer, count + sizeof(int), 0);

	delete sendBuffer;

	return result;
}