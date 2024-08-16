#ifndef BOCHAIN_COMMANDHANDLER_COMMANDHANDLER_HH
#define BOCHAIN_COMMANDHANDLER_COMMANDHANDLER_HH

class CommandHandler
{
   public:
	static CommandHandler &GetInstance();

   private:
	CommandHandler();
	CommandHandler(CommandHandler const &)		   = delete;
	auto operator=(CommandHandler const &) -> void = delete;

   public:
	void WaitForCommands();

   private:
};

CommandHandler &GetCommandHandler();

#endif	// BOCHAIN_COMMANDHANDLER_COMMANDHANDLER_HH