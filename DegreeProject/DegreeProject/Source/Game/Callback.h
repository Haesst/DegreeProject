#pragma once

using CallbackMember = void(void* data);

struct Callback
{
	CallbackMember* m_Action;
	void* data;
	unsigned int id;

	static unsigned int s_CallbackId;

	Callback(CallbackMember* functionPointer, void* data)
		: m_Action(functionPointer), data(data)
	{
		id = s_CallbackId++;
	}
};