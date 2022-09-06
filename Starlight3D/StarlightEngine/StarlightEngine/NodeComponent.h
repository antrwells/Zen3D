#pragma once
#include <vector>
#include <string>

/// <summary>
/// The node namespace is for all the types of nodes supported by the engine.
/// You can always extend one of them to create your own type of node.
/// Although node-components are designed for this also.
/// </summary>

	/// <summary>
	/// Node components are attached to Node3D/Entity/Etc classes to add extra functionality to the scene.
	/// </summary>
	class NodeComponent
	{
	public:

		NodeComponent();

		/// <summary>
		/// Called once when the node has started.
		/// </summary>
		virtual void Start()
		{
			mPlaying = true;
		}

		/// <summary>
		/// Returns true if the component is active.
		/// </summary>
		/// <returns></returns>
		bool Playing() {
			return mPlaying;
		}
		/// <summary>
		/// This is called once per frame, if the node has started.
		/// </summary>
		virtual void Update(){};
		/// <summary>
		/// This is called once per frame, if the node has started.
		/// </summary>
		virtual void Render(){};

		/// <summary>
		/// This is called once, once the node has ended it's run.
		/// </summary>
		virtual void End()
		{
			mPlaying = false;
		}
		/// <summary>
		/// called once, if the node has been paused.
		/// </summary>
		virtual void Pause(){};
		/// <summary>
		/// Called once, if the node is resumed after being paused.
		/// </summary>
		virtual void Resume() {};
		/// <summary>
		/// Called once per save, to allow you to save node/component specific data.
		/// </summary>
		virtual void SaveState() {};
		/// <summary>
		/// called once per load, to restore the specific state of a node/component.
		/// </summary>
		virtual void RestoreState() { };

		/// <summary>
		/// Allows you to set an arbitary piece of data, specific to the node.
		/// </summary>
		/// <param name="data"></param>
		void SetData(void* data)
		{
			mData = data;
		}
		/// <summary>
		/// Allows you to get the data asscoiated with the node.
		/// </summary>
		/// <returns></returns>
		void* GetData() {
			return mData;
		}


	private:

		/// <summary>
		/// The name of the component
		/// </summary>
		std::string name;
		bool mPlaying = false;
		void* mData = nullptr;


	};

