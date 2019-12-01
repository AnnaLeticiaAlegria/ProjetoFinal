import React from 'react'

import './index.less'

import Card from '../Card/index'

class CardContainer extends React.Component {

  state = {cardArray : [{name: 'map', text: 'Localização', data: {lat: 59.95, lng: 30.33}, isActive: true, isAlert: false},
              {name: 'heartRate', text: 'Frequência Cardíaca', data: '', isActive: true, isAlert: false},
              {name: 'fall', text: 'Queda', data: '', isActive: true, isAlert: false},
              {name: 'coGas', text: 'CO e Gases Inflamáveis', data: '', isActive: true, isAlert: false},
              {name: 'alert', text: 'Alertar', data: {}, isActive: true, isAlert: false}
          ], clientId: null}


  componentWillMount () {
    const { client } = this.props

    client.on("message", (topic, payload) => {
      // Log message
      console.log(topic);
      const topicRemoved = topic.replace("dados-sensores-1410427/data/", '')
      const clientId = topicRemoved.slice(0, topicRemoved.search("/"))
      const subTopic = topicRemoved.slice(topicRemoved.search("/") + 1, topicRemoved.length)

      this.setState({clientId: clientId})
      const payloadString = payload.toString()
      console.log(payloadString)

      switch (subTopic) {
        case "geo":
          const n1 = payloadString.search('"lat": ')
          const n2 = payloadString.search('"lng": ')
          this.setState(
            state => {
              const array = state.cardArray.map(item => {
                if(item.name === 'map') {
                  item.data = {lat: parseFloat(payloadString.substring(n1+7,n1+18)), lng: parseFloat(payloadString.substring(n2+7,n2+18))}
                }})
              return {
                array,
              };
          })
          break
        case "heart":
          this.setState(
            state => {
              const array = state.cardArray.map(item => {
                if(item.name === 'heartRate') {
                  item.data = payloadString
                }})
              return {
                array,
              };
          })
          break
        case "fall":
            this.setState(
              state => {
                const array = state.cardArray.map(item => {
                  if(item.name === 'fall') {
                    item.data = payloadString == 'true'
                    item.isAlert = item.data
                  }})
                return {
                  array,
                };
            })
            break
        case "gas":
            this.setState(
              state => {
                const array = state.cardArray.map(item => {
                  if(item.name === 'coGas') {
                    item.data = payloadString == 'true'
                    item.isAlert = item.data
                  }})
                return {
                  array,
                };
            })
            break
        default:
          break
      }
    });
  }

  changeState = (name, active=true, alert) => {
    this.setState(
      state => {
        const array = state.cardArray.map(item => {
          if(item.name === name) {
            if(active) {
              item.isActive = !item.isActive
            }
            else {
              item.isAlert = alert
            }
          }})
        return {
          array,
        };
      })
  }

  componentWillReceiveProps (nextProps) {
    console.log(nextProps)
    this.setState(
      state => {
        const array = state.cardArray.map(item => {
          if(item.name === 'map') {
            item.data = {lat: nextProps.lat, lng: nextProps.lng}
          }})
        return {
          array,
        };
      })
  }

  render (){
    const { cardArray, clientId } = this.state
    const { client } = this.props
    let sortList = cardArray.sort((a, b) => (a.isAlert < b.isAlert) ? 1 : (a.isAlert === b.isAlert) ? ((a.isActive < b.isActive) ? 1 : (a.isActive === b.isActive) ? ((a.text > b.text) ? 1 : -1 ) : -1 ) : -1)
    return (
      <div className="body__container">
        <div className="cards__container"> 
          {sortList.map(item => {
            return <Card key={item.name} info={item} changeState = {this.changeState} client={client} clientId={clientId}/>
          })}
        </div>
      </div>
    )
  }
}

export default CardContainer