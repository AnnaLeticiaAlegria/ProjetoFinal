import React from 'react'
import mqtt from 'mqtt'

import './index.less'

import CardContainer from '../CardContainer/index'
import Header from '../Header/index'

class App extends React.Component {
  
  state = {client: null}

  componentWillMount() {
    //this.getData()
    var client  = mqtt.connect('wss://test.mosquitto.org:8081')
    setTimeout(() => {
      client.publish("dados-sensores-1410427", "Hello world!");
    }, 1000);

    this.setState({client: client})
    
    client.on("connect",  () => {
      client.publish("dados-sensores-1410427", "Hello world!");
      client.subscribe("dados-sensores-1410427/#");
      console.log("Connected to MQTT Broker.");
    });
  }
  render (){
    const { client } = this.state
    return (
      <div className="app__wrapper">
        <Header />
        <CardContainer client={client}/>
      </div>
    )
  }
}

export default App